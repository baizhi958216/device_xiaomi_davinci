/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <cstdio>

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"

using std::string;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

bool has_eb_panel()
{
    std::ifstream cmdline("/proc/cmdline");
    string line;
    bool ret = false;

    std::getline(cmdline, line);
    if (line.find("dsi_ss_fhd_eb_f10_cmd_display") != string::npos)
        ret = true;

    cmdline.close();
    return ret;
}

void load_davinci_global() {
    property_override("ro.product.model", "Mi 9T");
    property_override("ro.build.product", "davinci");
    property_override("ro.product.device", "davinci");
    property_override("ro.product.mod_device", "davinci_global");
    property_override("ro.com.google.clientidbase", "android-xiaomi");
    property_override("ro.com.google.clientidbase.ms", "android-xiaomi-rev1");
    property_override("ro.boot.product.hardware.sku", "davinci_global");
}

void load_davinciin() {
    property_override("ro.product.model", "Redmi K20");
    property_override("ro.build.product", "davinciin");
    property_override("ro.product.device", "davinciin");
    property_override("ro.product.mod_device", "davinciin_in_global");
    property_override("ro.com.google.clientidbase", "android-xiaomi-rev1");
    property_override("ro.boot.product.hardware.sku", "davinciin");
}

void load_davinci_china() {
    property_override("ro.product.model", "Redmi K20");
    property_override("ro.build.product", "davinci");
    property_override("ro.product.device", "davinci");
    property_override("ro.boot.product.hardware.sku", "davinci_china");
}

void vendor_load_properties() {
    string region = android::base::GetProperty("ro.boot.hwc", "");

    if (region.find("GLOBAL") != string::npos) {
        load_davinci_global();
    } else if (region.find("INDIA") != string::npos) {
        load_davinciin();
    } else if (region.find("CN") != string::npos) {
        load_davinci_china();
    } else {
        LOG(ERROR) << __func__ << ": unexcepted region!";
    }

    property_override("persist.has.eb_panel", has_eb_panel() ? "true" : "false");
}
