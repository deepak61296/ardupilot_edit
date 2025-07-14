#include"AP_CustomModule.h"

const struct AP_Param::GroupInfo AP_CustomModule::var_info[] = {
    //@param: ENABLE
    //@DisplayName: Enable Custom Module
    //@Description: Enable custom module
    //@Values: 0:Disabled,1:Enabled
    //@User: Advanced
        AP_GROUPINFO("ENABLE", 1, AP_CustomModule, _enabled, 0),
    
    // @Param: RATE
    // @DisplayName: Custom Module Rate
    // @Description: Rate for custom module operations
    // @Range: 1 10
    // @Units: Hz
    // @User: Advanced
    AP_GROUPINFO("RATE", 2, AP_CustomModule, _rate, 1),
    
    // @Param: MODE
    // @DisplayName: Custom Module Mode
    // @Description: Mode for custom module
    // @Values: 0:LED Blink,1:Custom Messages,2:Servo Control
    // @Range: 0 2
    // @User: Advanced
    AP_GROUPINFO("MODE", 3, AP_CustomModule, _mode, 0),

    AP_GROUPEND

};

AP_CustomModule::AP_CustomModule(){
    AP_Param::setup_object_defaults(this, var_info);

}
// Initialization function - called once at startup
void AP_CustomModule::init()
{
    // Initialize our timing variable
    _last_update_ms = AP_HAL::millis();
}

// Update function - this is called repeatedly
void AP_CustomModule::update()
{
    // Check if module is disabled or rate is invalid
    if (!_enabled || _rate <= 0) {
        return;  // Exit early if disabled
    }
    
    // Calculate how often to run based on rate parameter
    // If rate = 2 Hz, interval = 1000/2 = 500ms
    uint32_t interval_ms = 1000 / _rate;
    
    // Check if enough time has passed since last update
    if (AP_HAL::millis() - _last_update_ms >= interval_ms) {
        
        // Do different actions based on mode parameter
        switch (_mode) {
            case 0: // LED Blink mode
                gcs().send_text(MAV_SEVERITY_INFO, "Library LED Blink! Rate: %d Hz", (int)_rate.get());
                break;
                
            case 1: // Custom Messages mode
                gcs().send_text(MAV_SEVERITY_INFO, "Library Custom Mode! Value: %d", (int)_rate.get());
                break;
                
            case 2: // Servo Control mode
                {
                    // Calculate servo position based on rate
                    // 1500 = center, rate*50 = offset
                    int16_t servo_value = 1500 + (_rate.get() * 50);
                    
                    // Set servo output (motor8 channel)
                    SRV_Channels::set_output_pwm(SRV_Channel::k_motor8, servo_value);
                    
                    // Send confirmation message
                    gcs().send_text(MAV_SEVERITY_INFO, "Library Servo set to %d", servo_value);
                }
                break;
        }
        
        // Update our timing variable for next iteration
        _last_update_ms = AP_HAL::millis();
    }
}
