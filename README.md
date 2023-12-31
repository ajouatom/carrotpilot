![image](https://github.com/ajouatom/carrotpilot/assets/43668841/ee65208b-9ca5-4861-942c-1dd1028f138f)
![image](https://github.com/ajouatom/carrotpilot/assets/43668841/4862230b-36c3-4ddd-8c59-943e73f9380c)
![image](https://github.com/ajouatom/carrotpilot/assets/43668841/c80e6fe7-5229-439f-a48d-6fefcc6c0e37)

What is carrotpilot?
------
CarrotPilot is based on FrogPilot. I've added various UI elements and features to FrogPilot's functionality.
당근파일럿은 FrogPilot을 기반으로 한다. 나는 FrogPilot에 APILOT의 다양한 UI와 기능을 추가하였다.

Why carrot?
------
In Korea, there's a humorous and casual way of saying 'of course' using the word for 'carrot,' which is 'dang-geun' in Korean. This is because 'dang-geun' (carrot) sounds very similar to 'dang-yeon-ha-da', which means 'of course.' It's a playful pun commonly used in everyday conversation among Koreans. So, if you hear someone responding with 'dang-geun,' they are affirmatively saying 'of course' in a light-hearted way. (한국말로 "당근"은 "당연하다"라는 표현으로 많이 사용한다. 당근파일럿에서는 "필요한 기능은 당연히 있다!" 라는 의미로 당근파일럿을 개발하였다.)

What is the useful features of CarrotPilot? 
------
**Radar Tracks: HKG cars only**
  - If you only turn on the "EnableRadarTracks" in the Toggle menu, it will be activated when the ignition is turned on.(레이더트랙을 지원하는 차량의 경우, 시동시 자체 활성화하는 기능이 있다.)

**SCC Wiring Modification Support: HKG cars only**
  - If the C-CAN wiring of the SCC modules is bypassed and connected to BUS2, long control using radar is supported.(레이더(SCC)모듈의 배선을 개조하여 BUS2에 연결한 경우, 롱컨을 지원한다.)
    
**APM: APILOT feature**
  - APM is ApilotManager. (APM은 ApilotManger을 말한다.)
  - APM is for Android Phone/Tablet.  (APM은 안드로이드 폰이나 태블릿에서 작동한다)
  - The features of CarrotPilot can be configured remotely via APM (당근의 기능은 APM으로 원격설정이 가능하다.)
    
**APN: Korea navigation only, APILOT feature**
  - When navigatgion is connected, you can use APN. (APM이 설치된 폰에 특정 네비게이션을 설치하면 APN모드를 사용할 수 있다)
  - It's a feature that automatically recuces speed in response to speed cameras and speed bumps for accident prevention. (오파의 NOO활성화 및 사고방지턱, 과속카메라에 대해 속도조절이 된다)
    
**APILOT UI: APM, APILOT feature**
  - You can experience a variety of APILOT UIs using APM. (APILOT의 다이내믹한 UI를 경험할 수 있다)
  - Path UI: When long control is turned off or on, and depending on whether it's in lane mode or laneless mode, it's possible to specify different shapes and colors for the path. Above all, the moving path shape looks cool. (레인모드, 레인리스모드, 크루즈OFF에 대한 패쓰경로표시 방식 및 색상을 변경할 수 있다)
  - Debug plotting: It's a plotting tool for tuning long control and steering control. (튜닝을 위하여 차량의 주행상황, 레이더등의 그래프를 지원한다)
    
**APILOT Traffic Stop/Go Mode: APILOT feature**
  - If you turn off the ConditionalExperimental Setting in FrogPilot, you can experience the APILOT Traffic Stop/Go Mode in CarrotPilot. (개구리파일럿기능의 ConditionalExperimental 을 Off하면 Apilot의 신호정지/출발 기능을 경험할 수 있다)

**Custom Acceleration Based on Speed: APM**
  - In APM, you can fine-tune the accleration based on driving speed.
  - Accelration Profile must be set to Normal in FrogPilot.

**Fuel Efficiency Control Feature: APM, APILOT feature**
  - It controls to slightly exceed the set speed, then naturally reduces acceleration at the set speed to lessen engine strain (Hybrid vehicles naturally switch to EV mode).
  - CruiseEcoControl(APM)
    
**Use Lane line mode:**
  - If the speed exceeds the set speed, it automatically switches to lane mode. This method uses lat_mpc
    
**Automatic Engage: HKG car only, APILOT feature**
  * Setting - APM/Carrot - Cruise (설정/당근/크루즈)
    * (HKG) Auto Cruise Control (자동 크루즈 제어)
      * 1: Enable Automatic cruise control (자동크루즈 제어 켜기)
      * 2: Enable Automatic cruies control, and auto cruise activate when the lead car approaches and you do not gas/brake press. (자동크루즈 제어켜기, 선행차가 가까와질때)
  * Setting - APM - Cruise
    * BrakeCruiseON: Stopping, Traffic (브레이크해제 크루즈ON)
      * At brake release, Automatic cruise activate when RED traffic light is detect or speed is faster than setting speed[GasCruise ON: Speed] 
    * GasCruise ON: Speed (엑셀 크루즈ON:속도)
      * If the gas pedal is pressed and the speed exceeds the set speed, Enage automatically activates
    * GasCruise OFF: Mode (엑셀 크루즈OFF:모드)
      * 1: When the speed is lower than the Gas Cruise ON speed, turning off the cruise control when the gas pedal is pressed. (엑셀크루즈ON속도보다 낮은속도로 주행중에 엑셀을 밟으면 크루즈를 끈다)
      * 2: Including item 1, and also turning off the cruise control when the gas pedal is pressed during deceleration due to traffic light detection. (1번항목포함하고, 신호감속중 엑셀을 밟으면 크루즈를 끈다)    
    * CruiseOnDist(0cm) (크루즈ON거리)
      * If driving without pressing either the brake or gas pedal, and the distance detected by the radar falls within the set value, then the cruise control is activated. But, if the value is negative, only a warning is issued." (브레이크/엑셀을 모두 밟지 않고 주행하다가 레이더거리가 설정값 이내에 들어오면 크루즈를 켠다. 단, 음수의 경우에는 경고만 한다)
  * SOFTHOLD: It's a feature that corresponds to the Autohold function. (오토홀드의 기능을 대신하는 기능)  

**Voice Recognition: APM, Korean only, APILOT feature, under construction!**
  - The connected APM supports voice recognition. It allows for lane changes and speed control using voice commands.

**Settings - Carrot**
  * Start
    * (HKG) SCC Module connected BUS2
    * (HKG) EnableRadarTracks
    * Adjust Sound Volume
    * Adjust Sound Volume, Engage
    * Mapbox Style
  * Cruise
    * (HKG) Auto Cruise control
    * Button: Cruise Button Mode
    * Button: Cruise Speed Unit
    * STOPPING: Traffic Stop Mode
    * CRUISE: Eco control
    * CRUISE: Auto ON distance
    * lol! too many..
  * Speed
  * Tuning
  * Disp
  * Path

**HKG차량을 위한 설정방법(롱컨이용가능차량)**
  * Setting - Device(장치)
    * Set to default(for HKG) : 누르고, 재시작(재부팅)    * 
  * Setting - Carrot(당근)
    * Start(시작) : 차량에 맞게 설정
    * 나머지는 APILOT과 거의 동일함.
  * Setting - Control
    * Adjustable Personalites : [Steering Wheel] or [Wheel + Button]
    * Conditional Experimental Mode : OFF (ON하면 Carrot의 기능작동정지)
    * Custom Driving Personalities : ON (변경하고 싶으면)
    * Fire the Babysitter : ON (OFF하고, DM을 끄면 banned 될수 있음)
    * Lateral Tuning: 맘대로
    * Longitudinal Tuning: ON
      * Acceleration Profile : Standard (다른것으로 하면 Carrot설정이 안됨)
      * Aggressive Acceleration With Lead : Off
      * Smoother Braking Behind Lead : Off
      * Increase Stopping Distance : Off
  * Setting - Navigation (맘대로)
    * ip주소:8082로 접속하여 Public/Secret Mapbox Key를 입력한다.
  * Setting - Vehicles (맘대로)
  * Setting - Visuals (맘대로)
---

![openpilot on the comma 3X](https://i.imgur.com/6l2qbf5.png)

Table of Contents
=======================

* [What is openpilot?](#what-is-openpilot)
* [What is FrogPilot?](#what-is-frogpilot)
* [Features](#features)
* [How to Install](#how-to-install)
* [Bug reports / Feature Requests](#bug-reports--feature-requests)
* [Discord](#discord)
* [Donations](#donations)
* [Credits](#credits)
* [Licensing](#licensing)

---

What is openpilot?
------

[openpilot](http://github.com/commaai/openpilot) is an open source driver assistance system. Currently, openpilot performs the functions of Adaptive Cruise Control (ACC), Automated Lane Centering (ALC), Forward Collision Warning (FCW), and Lane Departure Warning (LDW) for a growing variety of [supported car makes, models, and model years](docs/CARS.md). In addition, while openpilot is engaged, a camera-based Driver Monitoring (DM) feature alerts distracted and asleep drivers. See more about [the vehicle integration](docs/INTEGRATION.md) and [limitations](docs/LIMITATIONS.md).

<table>
  <tr>
    <td><a href="https://youtu.be/NmBfgOanCyk" title="Video By Greer Viau"><img src="https://i.imgur.com/1w8c6d2.jpg"></a></td>
    <td><a href="https://youtu.be/VHKyqZ7t8Gw" title="Video By Logan LeGrand"><img src="https://i.imgur.com/LnBucik.jpg"></a></td>
    <td><a href="https://youtu.be/VxiR4iyBruo" title="Video By Charlie Kim"><img src="https://i.imgur.com/4Qoy48c.jpg"></a></td>
    <td><a href="https://youtu.be/-IkImTe1NYE" title="Video By Aragon"><img src="https://i.imgur.com/04VNzPf.jpg"></a></td>
  </tr>
  <tr>
    <td><a href="https://youtu.be/iIUICQkdwFQ" title="Video By Logan LeGrand"><img src="https://i.imgur.com/b1LHQTy.jpg"></a></td>
    <td><a href="https://youtu.be/XOsa0FsVIsg" title="Video By PinoyDrives"><img src="https://i.imgur.com/6FG0Bd8.jpg"></a></td>
    <td><a href="https://youtu.be/bCwcJ98R_Xw" title="Video By JS"><img src="https://i.imgur.com/zO18CbW.jpg"></a></td>
    <td><a href="https://youtu.be/BQ0tF3MTyyc" title="Video By Tsai-Fi"><img src="https://i.imgur.com/eZzelq3.jpg"></a></td>
  </tr>
</table>


What is FrogPilot? 🐸
------

FrogPilot is my custom "Frog Themed" fork of openpilot that has been tailored to improve the driving experience for my 2019 Lexus ES 350. I resync with the latest version of master quite frequently, so this fork is always up to date. I also strive to make every commit I make easy to read and easily cherry-pickable, so feel free to use any of my features in your own personal forks in any way that you see fit!

------

FrogPilot was last updated on:

**December 8th, 2023**

Features
------

FrogPilot offers a wide range of customizable features that can be easily toggled on or off to suit your preferences. Whether you want a completely stock openpilot experience or want to add some fun and personal touches, FrogPilot has you covered! Some of the features include:

------
🎨 **Custom Themes:**

  - Themes included:
    - 🐸 Frog theme (with a bonus 🐐 sound effect)
    - <img src="https://images.emojiterra.com/google/noto-emoji/unicode-15/color/512px/1f1f7-1f1fa.png" width="20" height="18"> Russia / Joseph Stalin theme
    - 🔌 Tesla theme

  - 📢 Want to add a theme? Request one in the "feature-request" channel in the FrogPilot Discord!
------
🚀 **Conditional Experimental Mode:**

  - Auto-activates Experimental Mode under several conditions, including:
    - Approaching slower vehicles
    - Curve and stop light/stop sign detection
    - Driving below a set speed
    - Turn signal activation below 55mph for turn assistance
------
🎮 **Custom UI:**

  - Road UI Customizations:
    - Blind spot path to indicate a vehicle is in your blind spot within that respective lane
    - Compass that rotates according to the direction you're driving (Doesn't work on C3X)
    - Increase or decrease the lane line, path, and road edge widths
    - Path edge colors based on specific driving statuses
      - 🔵 Blue - Navigation active
      - 🟦 Light Blue - "Always On Lateral" active
      - 🟩 Light Green - Default
      - 🟠 Orange - Experimental Mode active
      - 🟡 Yellow - Conditional Experimental overridden
    - Steering wheel icons
      📢 Request your own in the "feature-request" channel!
    - Steering wheel in the onroad UI rotates alongside your physical steering wheel
    - "Unlimited" road UI that extends out as far as the model can see
------
📊 **Developer UI:**

  - Display various driving logics and device state metrics
  - Lane detection measuring the width of the adjacent lanes
  - Tap the "CPU"/"GPU" gauge to toggle between CPU and GPU monitoring
  - Tap the "MEMORY" gauge to toggle between RAM and storage monitoring
------
🛠 **Device Behaviors:**

  - Adjustable screen brightness
  - Device can operate offline indefinitely
  - Easy Panda firmware flashing via a "Flash Panda" button in the "Device" menu
  - Faster boot with prebuilt functionality
  - Set when the device will auto-shutdown to prevent battery drain
------
🚘 **Driving Behaviors:**

  - Adjust the set speed in increments of 5 by taping on "MAX" in the onroad UI
  - Lateral Adjustments:
    - Activate lateral control by simply pressing the "Cruise Control" button
    - Lateral control won't disengage on gas or brake
    - Nudgeless lane changes with lane detection to prevent driving into curbs or going offroad
    - [Pfeiferj's curvature adjustment](https://github.com/commaai/openpilot/pull/28118) for smoother curve handling
    - Precise turns by using turn desires when below the minimum lane change speed
    - [Twilsonco's NNFF](https://github.com/twilsonco/openpilot) for improved steering controls
  - Longitudinal Adjustments:
    - Aggressive acceleration following a lead vehicle from a stop
    - Enhanced stopping distances and braking behaviors
    - Speed Limit Controller to adjust your speed to the posted speed limit
      - With additional toggles to set offsets for the ranges of "0-34 mph", "35-54 mph", "55-64 mph", and "65-99 mph"
    - Sport and Eco acceleration modes
    - Vision Turn Speed Controller for smoother handling of curves
      - With additional toggles to fine tune it with your desired speed and curve detection sensitivity
  - Toggle Experimental Mode via the "Lane Departure Alert" button or by double tapping the screen
------
🚗 **Driving Personality Profiles:**

  - Ability to completely customize the following distance and jerk values for each profile
  - Adjustable via the "Distance" button on the steering wheel
    - Unsupported vehicle makes can use an Onroad UI button to switch between profiles with ease
------
⚡ **Model Switching:**

  - Select between various openpilot models to use your favorite model
------
🗺️ **Navigation:**

  - 3D buildings in the map panel
  - Navigate on openpilot without a comma prime subscription
  - Open Street Maps integration for speed limit control and road name view
------
🚙 **Vehicle Specific Additions:**

  - GM Volt support
  - Honda Clarity support
  - Lock doors automatically when in the drive gear for Toyota/Lexus
  - openpilot longitudinal control for GM vehicles without ACC
  - Pedal interceptor support for GM vehicles
  - SNG hack for Toyota's without stop and go functionality
  - Toyota/TSS2 tuning for smooth driving
  - ZSS support for the Toyota Prius
------
🚦 **Other Quality of Life Features:**

  - Disable the wide camera while in Experimental Mode (cosmetic only)
  - Green light alert
  - Hide the speed indicator by simply tapping on it
  - Numerical temperature gauge with the ability to use Fahrenheit by simply tapping on the gauge
  - Pause lateral control when turn signals are active
  - Sidebar displays by default for easy device monitoring
  - Silent Mode for a completely silent driving experience

How to Install
------

Easiest way to install FrogPilot is via this URL at the installation screen:

```
https://installer.comma.ai/FrogAi/FrogPilot
```
Be sure to capitalize the "F" and "P" in "FrogPilot" otherwise the installation will fail.

DO NOT install the "FrogPilot-Development" branch. I'm constantly breaking things on there so unless you don't want to use openpilot, NEVER install it!

![](https://i.imgur.com/wxKp3JI.png)

Bug reports / Feature Requests
------

If you encounter any issues or bugs while using FrogPilot, or if you have any suggestions for new features or improvements, please don't hesitate to reach out to me. I'm always looking for ways to improve the fork and provide a better experience for everyone!

To report a bug or request a new feature, feel free to make a post in the respective channel on the FrogPilot Discord. Provide as much detail as possible about the issue you're experiencing or the feature you'd like to see added. Photos, videos, log files, or other relevant information are very helpful!

I will do my best to respond to bug reports and feature requests in a timely manner, but please understand that I may not be able to address every request immediately. Your feedback and suggestions are valuable, and I appreciate your help in making FrogPilot the best it can be!

As for feature requests, these are my guidelines:

- Can I test it on my 2019 Lexus ES or are you up for testing it?
- How maintainable is it? Or will it frequently break with future openpilot updates?
- Is it not currently being developed by comma themselves? (i.e. Navigation)
- Will I personally use it or is it very niche?

Discord
------

[Join the FrogPilot Community Discord for easy access to updates, bug reporting, feature requests, future planned updates, and other FrogPilot related discussions!](https://l.linklyhq.com/l/1t3Il)

Donations
------

I DO NOT accept donations! So if anyone is claiming to be me or to be a part of FrogPilot and is asking for any type of financial compensation, IT IS A SCAM!

I work on FrogPilot on my own and is purely a passion project to refine my skills and to help improve openpilot for the community. I do not and will not ever expect any type of financial exchange for my work. The only thing I’ll ever ask for in return is constructive feedback!

Credits
------

* [AlexandreSato](https://github.com/AlexandreSato/openpilot)
* [Aragon7777](https://github.com/Aragon7777/openpilot)
* [Crwusiz](https://github.com/crwusiz/openpilot)
* [DragonPilot](https://github.com/dragonpilot-community/dragonpilot)
* [ErichMoraga](https://github.com/ErichMoraga/openpilot)
* [KRKeegan](https://github.com/krkeegan/openpilot)
* [Move-Fast](https://github.com/move-fast/openpilot)
* [OPGM](https://github.com/opgm/openpilot)
* [Pfeiferj](https://github.com/pfeiferj/openpilot)
* [Sunnyhaibin](https://github.com/sunnyhaibin/sunnypilot)
* [Twilsonco](https://github.com/twilsonco/openpilot)

Licensing
------

openpilot is released under the MIT license. Some parts of the software are released under other licenses as specified.

Any user of this software shall indemnify and hold harmless Comma.ai, Inc. and its directors, officers, employees, agents, stockholders, affiliates, subcontractors and customers from and against all allegations, claims, actions, suits, demands, damages, liabilities, obligations, losses, settlements, judgments, costs and expenses (including without limitation attorneys’ fees and costs) which arise out of, relate to or result from any use of this software by user.

**THIS IS ALPHA QUALITY SOFTWARE FOR RESEARCH PURPOSES ONLY. THIS IS NOT A PRODUCT.
YOU ARE RESPONSIBLE FOR COMPLYING WITH LOCAL LAWS AND REGULATIONS.
NO WARRANTY EXPRESSED OR IMPLIED.**

---

<img src="https://d1qb2nb5cznatu.cloudfront.net/startups/i/1061157-bc7e9bf3b246ece7322e6ffe653f6af8-medium_jpg.jpg?buster=1458363130" width="75"></img> <img src="https://cdn-images-1.medium.com/max/1600/1*C87EjxGeMPrkTuVRVWVg4w.png" width="225"></img>

[![openpilot tests](https://github.com/commaai/openpilot/workflows/openpilot%20tests/badge.svg?event=push)](https://github.com/commaai/openpilot/actions)
[![codecov](https://codecov.io/gh/commaai/openpilot/branch/master/graph/badge.svg)](https://codecov.io/gh/commaai/openpilot)
