/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011      Zynga Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

function RESpriteSheetGetSprite(aSpriteSheetName) {
    var cache = cc.SpriteFrameCache.getInstance(),
        plist = REResourceGetMCResourcesPath(aSpriteSheetName + '.plist'),
        spriteSheet = REResourceGetMCResourcesPath(aSpriteSheetName + '.png'),
        frameName = aSpriteSheetName.substring(13) + '-down-0';

        cache.addSpriteFrames(plist, spriteSheet);
        
    return cc.Sprite.createWithSpriteFrameName(frameName);
}

var AreaSelection = cc.Layer.extend({
    enable:false,
    beginRect:cc.rect(-100, -100, 0, 0),
    endRect:cc.rect(-100, -100, 0, 0),
    area:cc.rect(0, 0, 0, 0),
    
    draw: function () {
        if (!this.enable) {
            return;
        }

        var x1 = this.beginRect.origin.x,
            x2 = this.endRect.origin.x,
            y1 = this.beginRect.origin.y,
            y2 = this.endRect.origin.y,
            minX = Math.min(x1, x2),
            maxX = Math.max(x1, x2),
            minY = Math.min(y1, y2),
            maxY = Math.max(y1, y2),
            p1 = cc.p(minX, minY),
            p2 = cc.p(maxX + 32, minY),
            p3 = cc.p(maxX + 32, maxY + 32),
            p4 = cc.p(minX, maxY + 32);
            
            cc.renderContext.fillStyle = "rgba(255, 0, 64, 0.6)";
            cc.renderContext.strokeStyle = "rgba(255, 0, 64, 0.6)";
            
            this.origin = p1;
            this.area = cc.rect(minX, minY, maxX - minX + 32, maxY - minY + 32);
            cc.drawingUtil.drawPoly([p1, p2, p3, p4], 4, true, true);
    },
    
    getOrigin: function () {
        return this.area.origin;
    },
    
    getSize: function () {
        return this.area.size;
    },
    
    getArea: function () {
        return this.area;
    },
    
    showArea: function () {
        this.beginRect.origin = this.area.origin;
        this.beginRect.size = cc.size(32, 32);
        this.endRect.origin.x = this.area.origin.x + this.area.size.width - 32;
        this.endRect.origin.y = this.area.origin.y + this.area.size.height - 32;
        this.endRect.size = cc.size(32, 32);
    },
    hideArea: function () {
        this.beginRect = cc.rect(-100, -100, 0, 0);
        this.endRect = cc.rect(-100, -100, 0, 0);
    }
});

var TmxMap = cc.LayerColor.extend({
    tmxLayer:null,
    selector:null,
    placeholder:null,
    objects:null,
    NPCs:[],
    monsters:[],
    mapTileSize:cc.size(32, 32),
    hiddenLocation:cc.p(-100, -100),
    hideAction:cc.Hide.create(),
    showAction:cc.Show.create(),
    sceneData:{
    },
    hero:null,
    selectedSprite:null,
    selectedSpritePosition:null,
    areaSelection:null,
    NPCDidChange:null,
    monsterDidChange:null,

    init:function () {
        this._super();

        this.selector = cc.Sprite.create('res/selector.png');
        this.addChild(this.selector, 1);
        this.selector.setAnchorPoint(cc.PointZero);
        this.selector.setPosition(this.hiddenLocation);
        
        this.areaSelection = new AreaSelection;
        this.addChild(this.areaSelection, 3);

        this.setTouchEnabled(true);

        return true;
    },

    generateSceneData: function () {
        return this.sceneData;
    },

    updateTMXFile: function (aTMXFilepath) {
        if (!aTMXFilepath && this.tmxLayer !== null) {
            this.removeChild(this.tmxLayer)
        } else if (this.tmxLayer !== null) {
            this.removeChild(this.tmxLayer)
        }

        if (aTMXFilepath) {
            this.tmxLayer = cc.TMXTiledMap.create(aTMXFilepath);
            this.addChild(this.tmxLayer, 0);
            
            this.objects = this.tmxLayer.getObjectGroup('objects');
            
            //todo load npcs and monsters
        } else {
            this.tmxLayer = null;
            this.onMouseEnded();
            this.objects = null;
            
            this.NPCs = [];
            this.monsters = [];
        }
    },
    
    insertNPCDirectly: function (aNPCData, NPCProperties) {
        var sprite;
        
        /* 检查吃否已添加 */
        for (var i = 0, len = this.NPCs.length; i < len; ++i) {
            if (this.NPCs[i].userinfo.id == aNPCData.id) {
                return false;
            }
        }
        sprite = RESpriteSheetGetSprite(aNPCData.spriteSheet);
        this.addChild(sprite, this.NPCs.length + 3);
        this.NPCs.push(sprite);
        sprite.userinfo = aNPCData;
        sprite.properties = NPCProperties;
        sprite.setAnchorPoint(cc.PointZero);
        sprite.setPosition(NPCProperties.x, NPCProperties.y);
    },

    insertNPC: function (aNPCData) {
        /* 检查吃否已添加 */
        for (var i = 0, len = this.NPCs.length; i < len; ++i) {
            if (this.NPCs[i].userinfo.id == aNPCData.id) {
                return false;
            }
        }
        this.placeholder = RESpriteSheetGetSprite(aNPCData.spriteSheet);
        this.addChild(this.placeholder, this.NPCs.length + 3);
        this.placeholder.userinfo = aNPCData;
        this.placeholder.setAnchorPoint(cc.PointZero);
        this.placeholder.setPosition(cc.p(-100, -100));

        return true;
    },
    
    moveNPC: function () {
        if (this.selectedSprite !== null) {
            this.placeholder = this.selectedSprite;
        }
    },
    
    areaSelect: function () {
        this.areaSelection.enable = true;
    },

    viewLocationToMapLocation: function (screenLocation) {
        var map = this.tmxLayer,
            mapSize = map.getMapSize(),
            tileSize = map.getTileSize(),
            winSize = cc.Director.getInstance().getWinSize(),
            mapPosition = map.getPosition(),
            mapLocation = cc.p(screenLocation.x - mapPosition.x,
                               mapSize.height * tileSize.height - winSize.height + screenLocation.y + mapPosition.y),
            x = Math.floor(mapLocation.x / this.mapTileSize.width),
            y = Math.floor(mapLocation.y / this.mapTileSize.height);

        return cc.p(x, y);
    },

    mapLocationToViewLocation: function (mapLocation) {
        var map = this.tmxLayer,
            mapSize = map.getMapSize(),
            tileSize = map.getTileSize(),
            winSize = cc.Director.getInstance().getWinSize(),
            mapPosition = map.getPosition(),
            x = mapLocation.x * this.mapTileSize.width + mapPosition.x,
            y = mapSize.height * tileSize.height
                 - (mapLocation.y + 1)  * this.mapTileSize.height
                 + mapPosition.y;

        return cc.p(x, y);
    },

    onMouseMoved: function (e) {
        if (!this.tmxLayer) {
            return;
        }
        var map = this.tmxLayer,
            mapSize = map.getMapSize(),
            winSize = cc.Director.getInstance().getWinSize(),
            point = this.viewLocationToMapLocation(cc.p(e.offsetX, e.offsetY)),
            spritePosition = this.mapLocationToViewLocation(point);

        // console.log(spritePosition.x+', '+spritePosition.y)
        if (this.placeholder) {
            this.placeholder.setPosition(spritePosition);
        } else {
            this.selector.setPosition(spritePosition);   
        }
    },
    onMouseEnded: function (e) {
        if (this.placeholder) {
            this.placeholder.setPosition(this.hiddenLocation);
        } else {
            this.selector.setPosition(this.hiddenLocation);   
        }
    },

    onTouchesBegan:function (touches, event) {
        if (! this.tmxLayer) {
            return;
        }

        var pTouch = touches[0],
            touchLocation = cc.Director.getInstance().convertToGL(pTouch._point),
            point = this.viewLocationToMapLocation(touchLocation),
            selectedPoint = this.mapLocationToViewLocation(point),
            selectedRect = cc.rect(selectedPoint.x, selectedPoint.y, 32, 32);
            
//        puts(selectedPoint.x+' - '+selectedPoint.y)
            
        if (this.selectedSprite !== null
            && this.placeholder === this.selectedSprite) { //正在移动NPC
            return;
        }
        
        if (this.areaSelection.enable) {
            this.areaSelection.beginRect = selectedRect;
            this.areaSelection.endRect = selectedRect;
            $('#area-selection-confirm').addClass('hidden');
            return;
        }
        
        this.selectedSprite = null;
        if (! this.placeholder) { ///选择NPC
            for (var i = 0, len = this.NPCs.length; i < len; ++i) {
                if (cc.pointEqualToPoint(selectedPoint, this.NPCs[i].getPosition())) {
                    this.selectedSprite = this.NPCs[i];
                    this.selectedSpritePosition = selectedPoint;
                    //todo show area
                    this.areaSelection.area = cc.rect(this.selectedSprite.properties.x,
                                                      this.selectedSprite.properties.y,
                                                      this.selectedSprite.properties.w * 32,
                                                      this.selectedSprite.properties.h * 32);
                    this.areaSelection.showArea();
                    break;
                }
            }
        }
        
        //关闭菜单
        if (this.selectedSprite !== null) {
            $('#npc-menu').css({
                left: this.popupmenuPosition.x,
                top: this.popupmenuPosition.y
            }).removeClass('hidden');
        } else {
            $('#npc-menu').addClass('hidden');
        }
    },
    onTouchesMoved:function (touches, event) {
        var pTouch = touches[0],
            touchLocation = pTouch._point,
            prevLocation = pTouch._prevPoint,
            diff,
            map = this.tmxLayer,
            currentPos,
            newPos,
            mapSize,
            tileSize,
            winSize,
            w,
            h,
            ww,
            hh,
            NPC,
            point,
            selectedPoint,
            selectedRect;

        if (map === null || this.placeholder) {
            return;
        }
    
        touchLocation  = cc.Director.getInstance().convertToGL(touchLocation);
        prevLocation  = cc.Director.getInstance().convertToGL(prevLocation);
        
        
        if (this.areaSelection.enable) {
            point = this.viewLocationToMapLocation(touchLocation);
            selectedPoint = this.mapLocationToViewLocation(point);
            selectedRect = cc.rect(selectedPoint.x, selectedPoint.y, 32, 32);
            this.areaSelection.endRect = selectedRect;
            this.selectedSprite.setPosition(this.areaSelection.getOrigin());
            return;
        }
    
        diff = cc.pSub(touchLocation, prevLocation);
        diff.y = -diff.y;
        
        currentPos = map.getPosition();
        newPos = cc.pAdd(currentPos, diff);
        mapSize = map.getMapSize();
        tileSize = map.getTileSize();
        winSize = cc.Director.getInstance().getWinSize();
        w = mapSize.width * tileSize.width;
        h = mapSize.height * tileSize.height;
        ww = winSize.width;
        wh = winSize.height;
        if (newPos.x > 0) {
            newPos.x = 0;
        } else if (newPos.x - ww < -w) {
            newPos.x = ww - w;
        }
        if (newPos.y > 0) {
            newPos.y = 0;
        } else if (newPos.y - wh < -h) {
            newPos.y = wh - h;
        }
        diff = cc.pSub(newPos, map.getPosition())
        map.setPosition(newPos);

        for (var i = 0, len = this.NPCs.length; i < len; ++i) {
            NPC = this.NPCs[i];
            newPos = cc.pAdd(diff, NPC.getPosition());
            NPC.setPosition(newPos);
        }
    },
    onTouchesEnded:function (touches, event) {
        if (! this.tmxLayer) {
            return;
        }

        var pTouch = touches[0],
            touchLocation = cc.Director.getInstance().convertToGL(pTouch._point),
            point = this.viewLocationToMapLocation(touchLocation),
            selectedPoint = this.mapLocationToViewLocation(point),
            selectedRect = cc.rect(selectedPoint.x, selectedPoint.y, 32, 32);

        if (this.placeholder 
            && !cc.pointEqualToPoint(this.placeholder.getPosition(), this.hiddenLocation)
            && this.placeholder !== this.selectedSprite) { //添加NPCing
            this.NPCs.push(this.placeholder);
            this.placeholder.properties = {};
            this.placeholder.properties.requirement = [];
            this.placeholder.properties.x = selectedPoint.x;
            this.placeholder.properties.y = selectedPoint.y;
            this.placeholder.properties.w = 1;
            this.placeholder.properties.h = 1;
            this.NPCDidChange(this.placeholder.userinfo.id, this.placeholder.properties);
        }
        
        if (this.selectedSprite !== null) { //移动NPCing 
            this.areaSelection.beginRect = selectedRect;
            this.areaSelection.endRect = cc.rect(selectedRect.origin.x + this.selectedSprite.properties.w * 32 - 32,
                                                 selectedRect.origin.y + this.selectedSprite.properties.h * 32 - 32,
                                                 32,
                                                 32);
        }
        this.placeholder = null;
        
        if (this.areaSelection.enable) {
            this.areaSelection.endRect = selectedRect;
            //确认
            if (this.selectedSprite !== null) {
                $('#area-selection-confirm').css({
                    left: this.popupmenuPosition.x,
                    top: this.popupmenuPosition.y
                }).removeClass('hidden');
            } else {
                $('#area-selection-confirm').addClass('hidden');
            }
        }
    },
    onTouchesCancelled:function (touches, event) {
    }
});

var TmxMapScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new TmxMap;
        this._tmxLayer = layer;
        layer.init();
        this.addChild(layer);
    },

    getTMXTiledMapLayer: function () {
        return this._tmxLayer;
    }
});

