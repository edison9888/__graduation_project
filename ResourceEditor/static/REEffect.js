(function (window_) {
    window_.REEffect = {                    /* 效果 */
            kNoEffect:      0,              /* 无效果 */
            kCombustion:    REMakeEnum(0),  /* 燃烧 */
            kPoisoning:     REMakeEnum(1),  /* 中毒 */
            kDizziness:     REMakeEnum(2),  /* 眩晕 */
            kBlinding:      REMakeEnum(3),  /* 致盲 */
            kWeak:          REMakeEnum(4),  /* 虚弱 */
            kChaos:         REMakeEnum(5),  /* 混乱 */
        };

    window_.REEffectGetName = function (anEnumValue) {
        switch (anEnumValue) {
        case REEffect.kNoEffect:
            return '无效果';
        case REEffect.kCombustion:
            return '燃烧';
        case REEffect.kPoisoning:
            return '中毒';
        case REEffect.kDizziness:
            return '眩晕';
        case REEffect.kBlinding:
            return '致盲';
        case REEffect.kWeak:
            return '虚弱';
        case REEffect.kChaos:
            return '混乱';
        default:
            return '未知'; 
        }
    };

    /* data sources */
    window_.REEffects = [
        { name: REEffectGetName(REEffect.kNoEffect), value: REEffect.kNoEffect },
        { name: REEffectGetName(REEffect.kCombustion), value: REEffect.kCombustion },
        { name: REEffectGetName(REEffect.kPoisoning), value: REEffect.kPoisoning },
        { name: REEffectGetName(REEffect.kDizziness), value: REEffect.kDizziness },
        { name: REEffectGetName(REEffect.kBlinding), value: REEffect.kBlinding },
        { name: REEffectGetName(REEffect.kWeak), value: REEffect.kWeak },
        { name: REEffectGetName(REEffect.kChaos), value: REEffect.kChaos }
    ];
    
})(window);

