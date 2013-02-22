(function (window_) {
    window_.REWeaponUsage = {           /* 使用方式 */
        kOneHanded: REMakeEnum(0),      /* 单手武器 */
        kHanded:    REMakeEnum(1)       /* 双手武器 */
    };

    window_.REWeaponAttackType = {      /* 攻击类型 */
        kShortRange:    REMakeEnum(0),  /* 近程武器 */
        kLongRange:     REMakeEnum(1)   /* 远程武器 */
    };

    window_.REWeaponDamageType = {      /* 伤害类型 */
        kPuncture:      REMakeEnum(0),  /* 穿刺伤害 */
        kSlashing:      REMakeEnum(1),  /* 挥砍伤害 */
        kBludgeoning:   REMakeEnum(2),  /* 钝击伤害 */
        kImpact:        REMakeEnum(3),  /* 撞击伤害 */
        kExplosion:     REMakeEnum(4),  /* 爆炸伤害 */
    };
        
    window_.REWeaponUsageGetName = function (anEnumValue) {
        switch (anEnumValue) {
        case REWeaponUsage.kOneHanded:
            return '单手';
        case REWeaponUsage.kHanded:
            return '双手';
        default:
            return '未知'; 
        }
    };

    window_.REWeaponAttackTypeGetName = function (anEnumValue) {
        switch (anEnumValue) {
        case REWeaponAttackType.kShortRange:
            return '近程';
        case REWeaponAttackType.kLongRange:
            return '远程';
        default:
            return '未知'; 
        }
    };

    window_.REWeaponDamageTypeGetName = function (anEnumValue) {
        switch (anEnumValue) {
        case REWeaponDamageType.kPuncture:
            return '穿刺伤害';
        case REWeaponDamageType.kSlashing:
            return '挥砍伤害';
        case REWeaponDamageType.kBludgeoning:
            return '钝击伤害';
        case REWeaponDamageType.kImpact:
            return '撞击伤害';
        case REWeaponDamageType.kExplosion:
            return '爆炸伤害';
        default:
            return '未知伤害'; 
        }
    };

    /* data sources */
    window_.REWeaponUsages = [
        { name: REWeaponUsageGetName(REWeaponUsage.kOneHanded), value: REWeaponUsage.kOneHanded },
        { name: REWeaponUsageGetName(REWeaponUsage.kHanded), value: REWeaponUsage.kHanded }
    ];
    window_.REWeaponAttackTypes = [
        { name: REWeaponAttackTypeGetName(REWeaponAttackType.kShortRange), value: REWeaponAttackType.kShortRange },
        { name: REWeaponAttackTypeGetName(REWeaponAttackType.kLongRange), value: REWeaponAttackType.kLongRange }
    ];
    window_.REWeaponDamageTypes = [
        { name: REWeaponDamageTypeGetName(REWeaponDamageType.kPuncture), value: REWeaponDamageType.kPuncture },
        { name: REWeaponDamageTypeGetName(REWeaponDamageType.kSlashing), value: REWeaponDamageType.kSlashing },
        { name: REWeaponDamageTypeGetName(REWeaponDamageType.kBludgeoning), value: REWeaponDamageType.kBludgeoning },
        { name: REWeaponDamageTypeGetName(REWeaponDamageType.kImpact), value: REWeaponDamageType.kImpact },
        { name: REWeaponDamageTypeGetName(REWeaponDamageType.kExplosion), value: REWeaponDamageType.kExplosion }
    ];
    
})(window);
