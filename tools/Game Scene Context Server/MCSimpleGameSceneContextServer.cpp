//
//  MCSimpleGameSceneContextServer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSimpleGameSceneContextServer.h"

#include "JsonBox.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ostream>

#include "MCScene.h"
#include "MCEntrance.h"
#include "MCTeam.h"
#include "MCMercenary.h"
#include "MCEnemy.h"
#include "MCScript.h"

static MCSimpleGameSceneContextServer *__default_server = NULL;

static char kMCStopServerConstant[] = { 0x13, 'S', 'T', 'O', 'P', 0x13 };
static char kMCRequestDataConstant[] = { 0x13, 'R', 'E', 'Q', 'D', 0x13 };

static char kMCSceneDidChangeNotificationConstant[] = { 0x13, 'S', 'D', 'C', 'N', 0x13 };

const int kMCListeningPort = 4444;
const int kMCNotifyingPort = 4445;

static void *
listening(void *anUserdata)
{
    MCSimpleGameSceneContextServer *server = (MCSimpleGameSceneContextServer *) anUserdata;
    int sockfd = server->serverSockectFD();
	struct sockaddr_in clientAddr;
    socklen_t clientLength = sizeof(struct sockaddr_in);
    char buffer[4096];
    ssize_t n;
    
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *) &clientAddr, &clientLength);
        if (n < 0) {
            perror("接收数据错误");
        }
        if (memcmp(buffer, kMCStopServerConstant, sizeof(kMCStopServerConstant)) == 0) { /* 退出 */
            break;
        } else if (memcmp(buffer, kMCRequestDataConstant, sizeof(kMCRequestDataConstant)) == 0) { /* 请求战场数据 */
            /* 组织数据 */
            JsonBox::Object dataObject;
            server->getGameSceneContextData(dataObject);
            JsonBox::Value dataValue(dataObject);
            std::ostringstream dataStream;
            dataValue.writeToStream(dataStream, false, true);
            std::string data = dataStream.str();
            
            /* 发送数据 */
            n = sendto(sockfd, data.c_str(), data.size(), 0,
                       (const struct sockaddr *) &clientAddr, clientLength);
            if (n < 0) {
                perror("sendto");
            }
        }
    }
    
    return NULL;
}

MCSimpleGameSceneContextServer *
MCSimpleGameSceneContextServer::defaultSimpleGameSceneContextServer()
{
    if (__default_server == NULL) {
        __default_server = new MCSimpleGameSceneContextServer;
    }
    
    return __default_server;
}

void
MCSimpleGameSceneContextServer::runloop()
{
    if (! isRunning_) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        size_t optval = 1;
        
        if (sockfd < 0) {
            perror("不能创建socket！");
            return;
        }
        
        struct sockaddr_in addr;
        socklen_t addrLength = sizeof(addr);
        
        /* 重用地址 */
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
        
        /* 发送超时 */
        optval = 6000; /* 6秒 */
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
        
        memset(&addr, 0, addrLength);
        addr.sin_len = addrLength;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(sockfd, (const struct sockaddr *) &addr, addrLength) == -1) {
            perror("cannot bind address!");
            return;
        }
        serverSockectFD_ = sockfd;
        
        /* 创建线程 */
        pthread_attr_t pthreadAttr;
        int errCode = pthread_attr_init(&pthreadAttr);
        if (errCode != 0) {
            perror("线程创建失败！");
            return;
        }
        
        errCode = pthread_attr_setdetachstate(&pthreadAttr, PTHREAD_CREATE_DETACHED);
        if (errCode != 0) {
            pthread_attr_destroy(&pthreadAttr);
            perror("线程创建失败！");
            return;
        }
        
        errCode = pthread_create(&pid, &pthreadAttr, listening, this);
        if (errCode != 0) {
            perror("线程创建失败！");
            return;
        }
        
        isRunning_ = true;
    }
}

void
MCSimpleGameSceneContextServer::notifySceneDidChange()
{
    struct sockaddr_in addr;
    socklen_t addrLength = sizeof(addr);
    ssize_t n;
    
    if (logSockectFD_ == -1) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket创建失败");
            return;
        }
        logSockectFD_ = sockfd;
        size_t optval = 6000; /* 6秒 */
        
        /* 发送超时 */
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
    }
    
    memset(&addr, 0, addrLength);
    addr.sin_len = addrLength;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(kMCNotifyingPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    n = sendto(logSockectFD_, kMCSceneDidChangeNotificationConstant, sizeof(kMCSceneDidChangeNotificationConstant), 0,
               (const struct sockaddr *) &addr, addrLength);
    if (n < 1) {
        perror("发送刷新请求失败！");
    }
}

void
MCSimpleGameSceneContextServer::sendData(const char *data)
{
    struct sockaddr_in addr;
    socklen_t addrLength = sizeof(addr);
    ssize_t n;
    
    if (logSockectFD_ == -1) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket创建失败");
            return;
        }
        logSockectFD_ = sockfd;
        size_t optval = 6000; /* 6秒 */
        
        /* 发送超时 */
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
    }
    
    memset(&addr, 0, addrLength);
    addr.sin_len = addrLength;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(kMCNotifyingPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    n = sendto(logSockectFD_, data, strlen(data), 0,
               (const struct sockaddr *) &addr, addrLength);
    if (n < 1) {
        perror("发送消息失败！");
    }
}

void
MCSimpleGameSceneContextServer::getGameSceneContextData(JsonBox::Object &data)
{
    MCSceneContextManager *sceneContextManager = MCSceneContextManager::sharedSceneContextManager();
    MCSceneContext *sceneContext = sceneContextManager->currentContext();
    
    if (sceneContext) {
        /* 场景数据 */
        MCScene *scene = sceneContext->getScene();
        MCScenePackage *scenePackage = scene->getScenePackage();
        CCString *ccstring;
        
        /* ID String */
        mc_object_id_t scene_id = scenePackage->getID();
        const char c_str_scene_id[] = {
            scene_id.class_,
            scene_id.sub_class_,
            scene_id.index_,
            scene_id.sub_index_,
            0
        };
        data["id"] = JsonBox::Value(c_str_scene_id);
        
        /* type int */
        MCScenePackageType scenePackageType = scenePackage->getScenePackageType();
        if (scenePackageType == MCBattleFieldScenePackage) {
            data["scene-type"] = JsonBox::Value("战斗场景");
        } else if (scenePackageType == MCGameScenePackage) {
            data["scene-type"] = JsonBox::Value("通常场景");
        } else {
            data["scene-type"] = JsonBox::Value("未知场景");
        }
        
        /* internal int */
        data["internal"] =  scenePackage->isInternalScene()
                                ? JsonBox::Value("内部场景")
                                : JsonBox::Value("非内部场景");
        
        /* name String */
        data["name"] = JsonBox::Value(scenePackage->getName()->getCString());
        
        /* trigger String */
        ccstring = scenePackage->getTriggerFilepath();
        if (ccstring != NULL) {
            data["trigger"] = JsonBox::Value(ccstring->getCString());
        } else {
            data["trigger"] = JsonBox::Value("");
        }

        /* description String */
        data["description"] = JsonBox::Value(scenePackage->getDescription()->getCString());
        
        /* background Object */
        JsonBox::Object background;
        /* background["tmx"] String */
        background["tmx"] = JsonBox::Value(scenePackage->getTMXTiledMapPath()->getCString());
        /* background["sound"] String */
        background["sound"] = JsonBox::Value(scenePackage->getBackgroundMusicPath()->getCString());
        data["background"] = JsonBox::Value(background);

        /* scenes Object */
        JsonBox::Array entrances;
        CCDictionary *scenes = scenePackage->getScenes();
        CCDictElement *elem;
        MCEntrance *entrance;
        CCDICT_FOREACH(scenes, elem) {
            entrance = dynamic_cast<MCEntrance *>(elem->getObject());
            JsonBox::Object entranceObject;
            mc_object_id_t entrance_id = entrance->getID();
            const char c_str_entrance_id[] = {
                entrance_id.class_,
                entrance_id.sub_class_,
                entrance_id.index_,
                entrance_id.sub_index_,
                0
            };
            entranceObject["id"] = JsonBox::Value(c_str_entrance_id);
            entranceObject["name"] = JsonBox::Value(entrance->getName()->getCString());
            entrances.push_back(entranceObject);
        }
        data["scenes"] = JsonBox::Value(entrances);
        
        /* 对象数据 */
        /* team */
        MCTeam *team = MCTeam::sharedTeam();
        CCArray *teamRoles = team->getRoles();
        JsonBox::Array teamRolesJSON;
        CCObject *obj;
        MCRole *role;
        MCMercenary *mercenary;
        CCARRAY_FOREACH(teamRoles, obj) {
            role = dynamic_cast<MCRole *>(obj);
            JsonBox::Object roleObject;

            mc_object_id_t r_id = role->getID();
            const char c_str_r_id[] = {
                r_id.class_,
                r_id.sub_class_,
                r_id.index_,
                r_id.sub_index_,
                0
            };
            roleObject["id"] = JsonBox::Value(c_str_r_id);
            ccstring = role->getName();
            if (ccstring != NULL) {
                roleObject["name"] = JsonBox::Value(ccstring->getCString());
            } else {
                roleObject["name"] = JsonBox::Value("");
            }
            
            MCRole::MCRoleType roleType = role->getRoleType();
            if (roleType == MCRole::MCMercenary) {
                roleObject["role-type"] = JsonBox::Value("佣兵");
                mercenary = dynamic_cast<MCMercenary *>(role);
            } else {
                roleObject["role-type"] = JsonBox::Value("主角");
                mercenary = NULL;
            }
            
            /* MCTerrans等于MCTerrestrial。。。。 */
            MCRoleRace roleRace = role->getRoleRace();
            if (roleRace == MCTerrans) {
                roleObject["role-race"] = JsonBox::Value("人类");
            } else if (roleRace == MCTerrestrial) {
                roleObject["role-race"] = JsonBox::Value("陆行种");
            } else if (roleRace == MCTerrestrial) {
                roleObject["role-race"] = JsonBox::Value("飞行种");
            } else {
                roleObject["role-race"] = JsonBox::Value("神");
            }
            
            roleObject["hp"] = JsonBox::Value(role->getHP());
            roleObject["pp"] = JsonBox::Value(role->getPP());

            roleObject["exhausted"] = JsonBox::Value(role->isExhausted());
            roleObject["exhaustion"] = JsonBox::Value(role->getExhaustion());
            
            roleObject["tired"] = JsonBox::Value(role->getTired());
            roleObject["max-hp"] = JsonBox::Value(role->getMaxHP());
            roleObject["max-pp"] = JsonBox::Value(role->getMaxPP());
            
            MCRoleState roleState = role->getRoleState();
            if (MCNormalState == roleState) {
                roleObject["role-state"] = JsonBox::Value(kMCNormalState);
            } else {
                std::string state;
                if ((roleState & MCCurseState) == MCCurseState) {
                    state.append(kMCCurseState);
                }
                if ((roleState & MCParalysisState) == MCParalysisState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCParalysisState);
                }
                if ((roleState & MCVertigoState) == MCVertigoState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCVertigoState);
                }
                if ((roleState & MCPoisonedState) == MCPoisonedState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCPoisonedState);
                }
                if ((roleState & MCBlindingState) == MCBlindingState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCBlindingState);
                }
                if ((roleState & MCChaosState) == MCChaosState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCChaosState);
                }
                roleObject["role-state"] = JsonBox::Value(state);
            }
            roleObject["face"] = JsonBox::Value(role->getFace()->getCString());
            ccstring = role->getSpriteSheet();
            if (ccstring != NULL) {
                roleObject["sprite-sheet"] = JsonBox::Value(ccstring->getCString());
            } else {
                roleObject["sprite-sheet"] = JsonBox::Value("");
            }
            ccstring = role->getDefaultDialogue();
            if (ccstring != NULL) {
                roleObject["default-dialogue"] = JsonBox::Value(ccstring->getCString());
            } else {
                roleObject["default-dialogue"] = JsonBox::Value("");
            }

            MCAIState AIState = role->getAI()->getAIState();
            if (AIState == MCIdleState) {
                roleObject["ai-state"] = JsonBox::Value("空闲状态");
            } else if (AIState == MCCombatantStatus) {
                roleObject["ai-state"] = JsonBox::Value("战斗状态");
            } else if (AIState == MCRestingState) {
                roleObject["ai-state"] = JsonBox::Value("休息状态");
            } else if (AIState == MCAttackState) {
                roleObject["ai-state"] = JsonBox::Value("攻击状态");
            } else if (AIState == MCFollowingState) {
                roleObject["ai-state"] = JsonBox::Value("跟随状态");
            } else if (AIState == MCFleeState) {
                roleObject["ai-state"] = JsonBox::Value("逃跑状态");
            } else if (AIState == MCDeathState) {
                roleObject["ai-state"] = JsonBox::Value("死亡状态");
            } else {
                roleObject["ai-state"] = JsonBox::Value("未知状态");
            }
            
            MCScript *trigger = role->getTrigger();
            if (trigger != NULL) {
                ccstring = trigger->scriptPath_;
                if (ccstring != NULL) {
                    roleObject["trigger"] = JsonBox::Value(ccstring->getCString());
                } else {
                    roleObject["trigger"] = JsonBox::Value("");
                }
            } else {
                roleObject["trigger"] = JsonBox::Value("");
            }
            
            if (mercenary != NULL) {
                roleObject["dexterity"] = JsonBox::Value(mercenary->getDexterity());
                roleObject["ac"] = JsonBox::Value(mercenary->getAC());
                roleObject["armor-check-penalty"] = JsonBox::Value(mercenary->getArmorCheckPenalty());
                
                JsonBox::Object damageJSON;
                MCDice *damage = mercenary->getDamage();
                damageJSON["count"] = JsonBox::Value(damage->count());
                damageJSON["size"] = JsonBox::Value(damage->size());
                roleObject["damage"] = JsonBox::Value(damageJSON);
                
                roleObject["damage-bonus"] = JsonBox::Value(mercenary->getDamageBonus());
                
                JsonBox::Object criticalHitVisibleJSON;
                MCDiceRange range = mercenary->getCriticalHitVisible();
                criticalHitVisibleJSON["min"] = JsonBox::Value(JsonBox::Value(range.min));
                criticalHitVisibleJSON["max"] = JsonBox::Value(range.max);
                criticalHitVisibleJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                criticalHitVisibleJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                roleObject["critical-hit-visible"] = JsonBox::Value(criticalHitVisibleJSON);
                
                JsonBox::Object criticalHitInvisibleJSON;
                range = mercenary->getCriticalHitInvisible();
                criticalHitInvisibleJSON["min"] = JsonBox::Value(range.min);
                criticalHitInvisibleJSON["max"] = JsonBox::Value(range.max);
                criticalHitInvisibleJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                criticalHitInvisibleJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                roleObject["critical-hit-invisible"] = JsonBox::Value(criticalHitInvisibleJSON);
                
                roleObject["critical-hit"] = JsonBox::Value(mercenary->getCriticalHit());
                
                roleObject["distance"] = JsonBox::Value(mercenary->getDistance());
                roleState = mercenary->getEffect();
                
                bool hasEffect = true;
                if ((roleState & MCCurseState) == MCCurseState) {
                    roleObject["effect"] = JsonBox::Value(kMCCurseState);
                } else if ((roleState & MCParalysisState) == MCParalysisState) {
                    roleObject["effect"] = JsonBox::Value(kMCParalysisState);
                } else if ((roleState & MCVertigoState) == MCVertigoState) {
                    roleObject["effect"] = JsonBox::Value(kMCVertigoState);
                } else if ((roleState & MCPoisonedState) == MCPoisonedState) {
                    roleObject["effect"] = JsonBox::Value(kMCPoisonedState);
                } else if ((roleState & MCBlindingState) == MCBlindingState) {
                    roleObject["effect"] = JsonBox::Value(kMCBlindingState);
                } else if ((roleState & MCChaosState) == MCChaosState) {
                    roleObject["effect"] = JsonBox::Value(kMCChaosState);
                } else {
                    roleObject["effect"] = JsonBox::Value("");
                    hasEffect = false;
                }
                if (hasEffect) {
                    range = mercenary->getEffectCheck();
                    JsonBox::Object effectCheckJSON;
                    effectCheckJSON["min"] = JsonBox::Value(range.min);
                    effectCheckJSON["max"] = JsonBox::Value(range.max);
                    effectCheckJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                    effectCheckJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                    roleObject["effect-check"] = JsonBox::Value(effectCheckJSON);
                }
                
                roleObject["dying"] = JsonBox::Value(mercenary->getDying());
                roleObject["cost"] = JsonBox::Value(mercenary->getCost());
            }
            
            teamRolesJSON.push_back(roleObject);
        }
        
        /* others */
        JsonBox::Array otherRolesJSON;
        CCArray *enemies = sceneContext->getObjects();
        MCEnemy *enemy;
        CCARRAY_FOREACH(enemies, obj) {
            role = dynamic_cast<MCRole *>(obj);
            JsonBox::Object roleObject;
            mc_object_id_t r_id = role->getID();
            const char c_str_r_id[] = {
                r_id.class_,
                r_id.sub_class_,
                r_id.index_,
                r_id.sub_index_,
                0
            };
            
            roleObject["id"] = JsonBox::Value(c_str_r_id);
            roleObject["name"] = JsonBox::Value(role->getName()->getCString());
            
            MCRole::MCRoleType roleType = role->getRoleType();
            if (roleType == MCRole::MCEnemy) {
                roleObject["role-type"] = JsonBox::Value("敌人");
                enemy = dynamic_cast<MCEnemy *>(role);

            } else {
                roleObject["role-type"] = JsonBox::Value("NPC");
                enemy = NULL;
            }
            
            /* MCTerrans等于MCTerrestrial。。。。 */
            MCRoleRace roleRace = role->getRoleRace();
            if (roleRace == MCTerrans) {
                roleObject["role-race"] = JsonBox::Value("人类");
            } else if (roleRace == MCTerrestrial) {
                roleObject["role-race"] = JsonBox::Value("陆行种");
            } else if (roleRace == MCTerrestrial) {
                roleObject["role-race"] = JsonBox::Value("飞行种");
            } else {
                roleObject["role-race"] = JsonBox::Value("神");
            }
            
            roleObject["hp"] = JsonBox::Value(role->getHP());
            roleObject["pp"] = JsonBox::Value(role->getPP());
            
            roleObject["exhausted"] = JsonBox::Value(role->isExhausted());
            roleObject["exhaustion"] = JsonBox::Value(role->getExhaustion());
            
            roleObject["tired"] = JsonBox::Value(role->getTired());
            roleObject["max-hp"] = JsonBox::Value(role->getMaxHP());
            roleObject["max-pp"] = JsonBox::Value(role->getMaxPP());
            
            MCRoleState roleState = role->getRoleState();
            if (MCNormalState == roleState) {
                roleObject["role-state"] = JsonBox::Value(kMCNormalState);
            } else {
                std::string state;
                if ((roleState & MCCurseState) == MCCurseState) {
                    state.append(kMCCurseState);
                }
                if ((roleState & MCParalysisState) == MCParalysisState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCParalysisState);
                }
                if ((roleState & MCVertigoState) == MCVertigoState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCVertigoState);
                }
                if ((roleState & MCPoisonedState) == MCPoisonedState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCPoisonedState);
                }
                if ((roleState & MCBlindingState) == MCBlindingState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCBlindingState);
                }
                if ((roleState & MCChaosState) == MCChaosState) {
                    if (state.size() > 0) {
                        state.append("、");
                    }
                    state.append(kMCChaosState);
                }
                roleObject["role-state"] = JsonBox::Value(state);
            }
            ccstring = role->getFace();
            if (ccstring != NULL) {
                roleObject["face"] = JsonBox::Value(ccstring->getCString());
            } else {
                roleObject["face"] = JsonBox::Value("");
            }
            roleObject["sprite-sheet"] = JsonBox::Value(role->getSpriteSheet()->getCString());
            ccstring = role->getDefaultDialogue();
            if (ccstring != NULL) {
                roleObject["default-dialogue"] = JsonBox::Value(ccstring->getCString());
            } else {
                roleObject["default-dialogue"] = JsonBox::Value("");
            }
            
            MCAIState AIState = role->getAI()->getAIState();
            if (AIState == MCIdleState) {
                roleObject["ai-state"] = JsonBox::Value("空闲状态");
            } else if (AIState == MCCombatantStatus) {
                roleObject["ai-state"] = JsonBox::Value("战斗状态");
            } else if (AIState == MCRestingState) {
                roleObject["ai-state"] = JsonBox::Value("休息状态");
            } else if (AIState == MCAttackState) {
                roleObject["ai-state"] = JsonBox::Value("攻击状态");
            } else if (AIState == MCFollowingState) {
                roleObject["ai-state"] = JsonBox::Value("跟随状态");
            } else if (AIState == MCFleeState) {
                roleObject["ai-state"] = JsonBox::Value("逃跑状态");
            } else if (AIState == MCDeathState) {
                roleObject["ai-state"] = JsonBox::Value("死亡状态");
            } else {
                roleObject["ai-state"] = JsonBox::Value("未知状态");
            }
            
            
            MCScript *trigger = role->getTrigger();
            if (trigger != NULL) {
                ccstring = trigger->scriptPath_;
                if (ccstring != NULL) {
                    roleObject["trigger"] = JsonBox::Value(ccstring->getCString());
                } else {
                    roleObject["trigger"] = JsonBox::Value("");
                }
            } else {
                roleObject["trigger"] = JsonBox::Value("");
            }

            if (enemy != NULL) {
                roleObject["dexterity"] = JsonBox::Value(enemy->getDexterity());
                roleObject["ac"] = JsonBox::Value(enemy->getAC());
                roleObject["armor-check-penalty"] = JsonBox::Value(enemy->getArmorCheckPenalty());
                
                JsonBox::Object damageJSON;
                MCDice *damage = enemy->getDamage();
                damageJSON["count"] = JsonBox::Value(damage->count());
                damageJSON["size"] = JsonBox::Value(damage->size());
                roleObject["damage"] = JsonBox::Value(damageJSON);
                
                roleObject["damage-bonus"] = JsonBox::Value(enemy->getDamageBonus());
                
                JsonBox::Object criticalHitVisibleJSON;
                MCDiceRange range = enemy->getCriticalHitVisible();
                criticalHitVisibleJSON["min"] = JsonBox::Value(range.min);
                criticalHitVisibleJSON["max"] = JsonBox::Value(range.max);
                criticalHitVisibleJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                criticalHitVisibleJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                roleObject["critical-hit-visible"] = JsonBox::Value(criticalHitVisibleJSON);
                
                JsonBox::Object criticalHitInvisibleJSON;
                range = enemy->getCriticalHitInvisible();
                criticalHitInvisibleJSON["min"] = JsonBox::Value(range.min);
                criticalHitInvisibleJSON["max"] = JsonBox::Value(range.max);
                criticalHitInvisibleJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                criticalHitInvisibleJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                roleObject["critical-hit-invisible"] = JsonBox::Value(criticalHitInvisibleJSON);
                
                roleObject["critical-hit"] = JsonBox::Value(enemy->getCriticalHit());
                
                roleObject["distance"] = JsonBox::Value(enemy->getDistance());
                roleState = enemy->getEffect();
                
                bool hasEffect = true;
                if ((roleState & MCCurseState) == MCCurseState) {
                    roleObject["effect"] = JsonBox::Value(kMCCurseState);
                } else if ((roleState & MCParalysisState) == MCParalysisState) {
                    roleObject["effect"] = JsonBox::Value(kMCParalysisState);
                } else if ((roleState & MCVertigoState) == MCVertigoState) {
                    roleObject["effect"] = JsonBox::Value(kMCVertigoState);
                } else if ((roleState & MCPoisonedState) == MCPoisonedState) {
                    roleObject["effect"] = JsonBox::Value(kMCPoisonedState);
                } else if ((roleState & MCBlindingState) == MCBlindingState) {
                    roleObject["effect"] = JsonBox::Value(kMCBlindingState);
                } else if ((roleState & MCChaosState) == MCChaosState) {
                    roleObject["effect"] = JsonBox::Value(kMCChaosState);
                } else {
                    roleObject["effect"] = JsonBox::Value("");
                    hasEffect = false;
                }
                if (hasEffect) {
                    range = enemy->getEffectCheck();
                    JsonBox::Object effectCheckJSON;
                    effectCheckJSON["min"] = JsonBox::Value(range.min);
                    effectCheckJSON["max"] = JsonBox::Value(range.max);
                    effectCheckJSON["count"] = JsonBox::Value(MCDiceCount(range.dice));
                    effectCheckJSON["size"] = JsonBox::Value(MCDiceSize(range.dice));
                    roleObject["effect-check"] = JsonBox::Value(effectCheckJSON);
                }
            }
            
            otherRolesJSON.push_back(roleObject);
        }
        
        JsonBox::Object rolesJSON;
        rolesJSON["team"] = JsonBox::Value(teamRolesJSON);
        rolesJSON["others"] = JsonBox::Value(otherRolesJSON);
        data["roles"] = JsonBox::Value(rolesJSON);
    }
}
