#pragma once

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <GCS_MAVLink/GCS.h>
#include<SRV_Channel/SRV_Channel.h>

class AP_CustomModule {
    public:
        AP_CustomModule();
        //initialize the module
        void init();
        //update fun
        void update();

        //param access
        bool enabled() const { return _enabled; }
        uint8_t get_rate() const { return _rate; }
        uint8_t get_mode() const { return _mode;}
        static const struct AP_Param::GroupInfo var_info[];

    private:
        AP_Int8 _enabled;
        AP_Int8 _rate;
        AP_Int8 _mode;

        uint32_t _last_update_ms;
};



