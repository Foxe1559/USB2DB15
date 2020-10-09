//
// Created by Kitsune on 8/21/2020.
// Modified by Frank_fjs on 10/9/2020. 
//

#include "PS3Controller.h"
#include "device_descriptor.h"

/**
 * Create a new PS3Controller
 *
 * @param p Pointer to the PS3 Controller to Wrap
 */
PS3Controller::PS3Controller(PS3USB *p) {
  ps3usb = p;
}

/**
 * Gets if the button has been clicked since the last check
 *
 * This function uses the aliases provide by the Host Shield Library
 * to determine if a given button has been clicked. Only returns true
 * once per button press
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the button has been clicked
 */

/**
 * PS3 analog | range of 0 to 255 | 117 to 137 = centre / OFF 
 * UP    LeftHatY | 0   = FULL-UP    - first registers at 116 
 * DOWN  LeftHatY | 255 = FULL-DOWN  - first registers at 138 
 * LEFT  LeftHatX | 0   = FULL-LEFT  - first registers at 116 
 * RIGHT LeftHatX | 255 = FULL-RIGHT - first registers at 138 
 * 
 * #define PS3_ANALOG_SENSITIVITY
 * 
 * Lower value registers analog nub sooner 
 * Higher value registers analog nub later 
 * valid range of 1 to 116
 * 
 * bool disableAnalog;
 * 
 * Safeguard to prevent controllers with digital buttons 
 * connected to analog outputs from registering false alerts
 */
#define PS3_ANALOG_SENSITIVITY 32 
bool disableAnalog;

bool PS3Controller::GetButtonClick(uint8_t button) {
  // Values of 0 trigger LeftHatX (DOWN) and LeftHatY (UP)
  // Controllers with digital buttons acting as analog buttons output 0 constantly
  // Thus we need to check for this scenario and disable analog output
  if(ps3usb->getAnalogHat(LeftHatX) == 0 && ps3usb->getAnalogHat(LeftHatY) == 0) 
    disableAnalog = true; 
  else 
    disableAnalog = false; 

  switch(button) {
    case BUTTON_UP:
      if(disableAnalog) 
        return ps3usb->getButtonClick(UP);  
      else 
        return ps3usb->getButtonClick(UP) || ps3usb->getAnalogHat(LeftHatY) < 117 - PS3_ANALOG_SENSITIVITY;
    case BUTTON_DOWN:
      return ps3usb->getButtonClick(DOWN) || ps3usb->getAnalogHat(LeftHatY) > 137 + PS3_ANALOG_SENSITIVITY;
    case BUTTON_LEFT:
      if(disableAnalog) 
        return ps3usb->getButtonClick(LEFT);  
      else 
        return ps3usb->getButtonClick(LEFT) || ps3usb->getAnalogHat(LeftHatX) < 117 - PS3_ANALOG_SENSITIVITY;
    case BUTTON_RIGHT:
      return ps3usb->getButtonClick(RIGHT) || ps3usb->getAnalogHat(LeftHatX) > 137 + PS3_ANALOG_SENSITIVITY;
    case BUTTON_START:
      return ps3usb->getButtonClick(START);
    case BUTTON_COIN:
      return ps3usb->getButtonClick(BACK);
    case BUTTON_1:
      return ps3usb->getButtonClick(SQUARE);
    case BUTTON_2:
      return ps3usb->getButtonClick(TRIANGLE);
    case BUTTON_3:
      return ps3usb->getButtonClick(R1);
    case BUTTON_4:
      return ps3usb->getButtonClick(X);
    case BUTTON_5:
      return ps3usb->getButtonClick(CIRCLE);
    case BUTTON_6:
      return ps3usb->getButtonClick(R2);
    case BUTTON_7:
      return ps3usb->getButtonClick(L1);
    case BUTTON_8:
      return ps3usb->getButtonClick(L2);
    case BUTTON_9:
      return ps3usb->getButtonClick(R3);
    case BUTTON_10:
      return ps3usb->getButtonClick(L3);
  }
  return false;
}

/**
 * Gets the State(pressed or not) of a given button
 *
 * This function uses the aliases provide by the Host Shield Library
 * to determine if a given button is pressed on the controller
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the requested button is pressed
 */
bool PS3Controller::GetButtonState(uint8_t button) {
  // Values of 0 trigger LeftHatX (DOWN) and LeftHatY (UP)
  // Controllers with digital buttons acting as analog buttons output 0 constantly
  // Thus we need to check for this scenario and disable analog output  
  if(ps3usb->getAnalogHat(LeftHatX) == 0 && ps3usb->getAnalogHat(LeftHatY) == 0) 
    disableAnalog = true; 
  else
    disableAnalog = false; 

  switch(button) {
    case BUTTON_UP:
      if(disableAnalog) 
        return ps3usb->getButtonPress(UP);  
      else 
        return ps3usb->getButtonPress(UP) || ps3usb->getAnalogHat(LeftHatY) < 117 - PS3_ANALOG_SENSITIVITY; 
    case BUTTON_DOWN:
      return ps3usb->getButtonPress(DOWN) || ps3usb->getAnalogHat(LeftHatY) > 137 + PS3_ANALOG_SENSITIVITY;
    case BUTTON_LEFT:
      if(disableAnalog) 
        return ps3usb->getButtonPress(LEFT);  
      else 
        return ps3usb->getButtonPress(LEFT) || ps3usb->getAnalogHat(LeftHatX) < 117 - PS3_ANALOG_SENSITIVITY;  
    case BUTTON_RIGHT:
      return ps3usb->getButtonPress(RIGHT) || ps3usb->getAnalogHat(LeftHatX) > 137 + PS3_ANALOG_SENSITIVITY;
    case BUTTON_START:
      return ps3usb->getButtonPress(START);
    case BUTTON_COIN:
      return ps3usb->getButtonPress(BACK);
    case BUTTON_1:
      return ps3usb->getButtonPress(SQUARE);
    case BUTTON_2:
      return ps3usb->getButtonPress(TRIANGLE);
    case BUTTON_3:
      return ps3usb->getButtonPress(R1);
    case BUTTON_4:
      return ps3usb->getButtonPress(X);
    case BUTTON_5:
      return ps3usb->getButtonPress(CIRCLE);
    case BUTTON_6:
      return ps3usb->getButtonPress(R2);
    case BUTTON_7:
      return ps3usb->getButtonPress(L1);
    case BUTTON_8:
      return ps3usb->getButtonPress(L2);
    case BUTTON_9:
      return ps3usb->getButtonPress(R3);
    case BUTTON_10:
      return ps3usb->getButtonPress(L3);
  }
  return false;
}

/**
 * Checks if an Official PS3 controller is connected to the system
 *
 * Checks if a PS3 controller is connected to the system. The Vid and Pid
 * of PS3 Controllers is defined in the Host Shield Library.  Any
 * Controllers not defined as PS3 controllers in the library will be
 * treated as generic HID controllers. See the RAP3 PS3 driver for
 * HID support
 *
 * @return true if an PS3 controller is connected to the system
 */
bool PS3Controller::Connected() {
  return ps3usb->PS3Connected;
}

/**
 * Gets the Vendor ID for the controller
 * @return The Controller's Vendor ID
 */
uint16_t PS3Controller::GetVid() {
  return PS3_VID;  // The PS3 controller has a static VID
}

/**
 * Gets the Product ID for the controller
 * @return The Controller's Product ID
 */
uint16_t PS3Controller::GetPid() {
  return PS3_PID; // The PS3 controller has a static PID
}
