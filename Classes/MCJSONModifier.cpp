//
//  MCJSONModifier.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <memory.h>

#include "MCJSONModifier.h"

static mc_byte_t buffer[4096];
static mc_byte_t header[4] = {0, 'J', 13, 24};

static void
hide(const char *aFilepath)
{
    FILE *in = fopen(aFilepath, "r");
    const char *tempfile = tempnam("/tmp", "zzj");
    FILE *out = fopen(tempfile, "wb");
    size_t n;
    

    n = fwrite(header, sizeof(mc_byte_t), sizeof(header), out);
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        n = fread(buffer, sizeof(mc_byte_t), sizeof(buffer), in);
        if (n == 0) {
            break;
        }
        fwrite(buffer, sizeof(mc_byte_t), n, out);
    }
    fclose(out);
    fclose(in);
    rename(tempfile, aFilepath);
}

static void
show(const char *aFilepath)
{
    FILE *in = fopen(aFilepath, "r");
    const char *tempfile = tempnam("/tmp", "zzj");
    FILE *out = fopen(tempfile, "wb");
    size_t n;
    
    fseek(in, sizeof(header), SEEK_SET);
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        n = fread(buffer, sizeof(mc_byte_t), sizeof(buffer), in);
        if (n == 0) {
            break;
        }
        fwrite(buffer, sizeof(mc_byte_t), n, out);
    }
    fclose(out);
    fclose(in);
    rename(tempfile, aFilepath);
}

void
MCJSONModifier::setJSONFileVisible(const char         *aFilepath,
                                   MCJSONVisibleState aState)
{
    FILE *in = fopen(aFilepath, "r");
    
    if (in == NULL) {
        return;
    }
    
    fread(buffer, sizeof(mc_byte_t), sizeof(header), in);
    fseek(in, 0, SEEK_SET);
    fclose(in);
    
    if (aState == MCJSONVisible) {
        printf("MCJSONVisible\n");
        /* 验证 */
        if (memcmp(buffer, header, sizeof(header)) == 0) { /* Yup！ */
            show(aFilepath);
        }
    } else if (aState == MCJSONInvisible) {
        /* 验证 */
        if (memcmp(buffer, header, sizeof(header)) != 0) { /* Oh，NO！ */
            hide(aFilepath);
        }
    }
    
}