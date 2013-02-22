(function (window_) {
    window_.REOre = {                       /* 矿石 */
            kCopper:        REMakeEnum(0),  /* 铜矿 */
            kIron:          REMakeEnum(1),  /* 铁矿 */
            kSilver:        REMakeEnum(2),  /* 银矿 */
            kGold:          REMakeEnum(3),  /* 金矿 */
            kCrystal:       REMakeEnum(4),  /* 水晶矿 */
            kAmethyst:      REMakeEnum(5),  /* 紫水晶矿 */
            kBlocks:        REMakeEnum(6),  /* 黑金石矿 */
            kOrichalcos:    REMakeEnum(7),  /* 奥利哈刚 */
            kUnknow:        REMakeEnum(8)   /* 未知矿石，用于特殊装备 */
        };

    window_.REOreGetName = function (anEnumValue) {
        switch (anEnumValue) {
        case REOre.kCopper:
            return '铜矿';
        case REOre.kIron:
            return '铁矿';
        case REOre.kSilver:
            return '银矿';
        case REOre.kGold:
            return '金矿';
        case REOre.kCrystal:
            return '水晶矿';
        case REOre.kAmethyst:
            return '紫水晶矿';
        case REOre.kBlocks:
            return '黑金石矿';
        case REOre.kOrichalcos:
            return '奥利哈刚';
        case REOre.kUnknow:
            return '未知矿石'; 
        default:
            return '未知'; 
        }
    };

    /* data sources */
    window_.REOres = [
        { name: REOreGetName(REOre.kCopper), value: REOre.kCopper },
        { name: REOreGetName(REOre.kIron), value: REOre.kIron },
        { name: REOreGetName(REOre.kSilver), value: REOre.kSilver },
        { name: REOreGetName(REOre.kGold), value: REOre.kGold },
        { name: REOreGetName(REOre.kCrystal), value: REOre.kCrystal },
        { name: REOreGetName(REOre.kAmethyst), value: REOre.kAmethyst },
        { name: REOreGetName(REOre.kBlocks), value: REOre.kBlocks },
        { name: REOreGetName(REOre.kOrichalcos), value: REOre.kOrichalcos },
        { name: REOreGetName(REOre.kUnknow), value: REOre.kUnknow }
    ];
    
})(window);

