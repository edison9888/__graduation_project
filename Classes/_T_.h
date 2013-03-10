//
//  _T_.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-7.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation__T__h
#define Military_Confrontation__T__h

class _T_ {
public:
    _T_() {
        age = 13;
    };
    int age;
    
    static _T_ *create() {
        return new _T_;
    }
    
    void setAge(int anAge) {
        age = anAge;
    }
    
    int getAge() {
        return age;
    }
};

#endif
