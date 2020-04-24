/*************************************************************
  This is a DEMO. You can use it only for development and testing.
  You should open Setting.h and modify General options.

  If you would like to add these features to your product,
  please contact Blynk for Businesses:

                   http://www.blynk.io/

 *************************************************************/

#define USE_NODE_MCU_BOARD        // Comment out the boards you are not using

#define APP_DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial

#include "BlynkProvisioning.h"


//===================================OUTSIDE LIBRARY===========================================

//////Additional lib added for serial print // arduino communcation // timer

#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

//LIBRARY FOR WEBSOCKETS
#include <WebSocketsServer.h>
#include <WiFiClient.h>
WebSocketsServer webSocket=WebSocketsServer(88);
DNSServer dnServer; 

//===================================VARIABLES===========================================

String JSONtxt;                          // JSONtxt txt is string for sending data to webserver


bool ipprint=0;
int d1, d2, d3, d4, d5; // to store old status of OUTPIN
//int io1val=0,io2val=0,io3val=0,io4val=0,io5val=0; //to store xml output value send via webserver


// PAGEONLOAD VARIABLE TO DETECT WHETHER THE PAGE IS LOADING FIRST TIME OR SECOND TIME();
//if the page is loading first time then send full information of switches and second time which ever is changed.
bool pageonload=0;
//jsonprint is on or off.
bool jsonprint=0;
//Variable for dimmer send data to arduino for setting dimmer
//int pinValue = 1;
String v2arduino; // values sending to arduino


String myString; // complete message from arduino, which consist of fan speed
char rdata; // received characters from arduino via serial
int valueforslider = 1;  //store rdata value for future use
int chk_valforslider = 200; // to check the value for slider is changed before sending to arduino


//TIMER INTILATISING AND DECLARATION
BlynkTimer timer;

void checkPressButton(); //Funtion for Press switch
void check_normal_Btn();   // Funtion for Normal Switch

bool switch_flag=0; // Variable for running only one time for intilaising timmer funtion; 

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);


const int btnPin1 = 12;      // pin for physical push button switch.
const int btnPin2 = 14;      // pin for physical push button switch.
const int btnPin3 = 13;      // pin for physical push button switch.
const int btnPin4 = 2;       // pin for physical push button switch.

const int OutPin1 =  5;      // pin for relay output.
const int OutPin2 =  16;      // pin for relay output.
const int OutPin3 =  4;      // pin for relay output.
const int OutPin4 =  0;      // pin for relay output.


int btnPin1State = LOW;           // ON
int OutPin1State = HIGH;        // OFF
int btnPin2State = LOW;           // ON
int OutPin2State = HIGH;        // OFF
int btnPin3State = LOW;           // ON
int OutPin3State = HIGH;        // OFF
int btnPin4State = LOW;           // ON
int OutPin4State = HIGH;        // OFF


int btnPin1old = 2;
int btnPin2old = 2;
int btnPin3old = 2;
int btnPin4old = 2;

//================================   FUNCTIONS   ===========================================


//////FUNCTION FOR WEBSERVER HTML PAGES CONFIG PAGE

const char webSite[] PROGMEM = // #xxxxxxxx# are later to be changed in c function WebsiteContent()
R"=====(
    <!DOCTYPE HTML><HTML>
    <META name='viewport' content='width=device-width, initial-scale=1'>
    <html>
      <style>
         .btn{
          position:relative;
          float:left;
          margin-right:100px;
          margin-bottom:100px;
          font-size:20px;
          font-weight: bold;
          width:90px;
          height:90px;
          border-radius: 90px;
          padding:10px;
          color:white;
          text-align:center;
          background-color:Orange;
          }
          body {
  width: 100%;
  margin: 0;
  padding: 0;
  background: #121212;
  padding: 30px;
}
a,
input[type="radio"] + label {
  text-align: center;
  display: inline-block;
  text-decoration: none;
  cursor: pointer;
  font-family: "Trebuchet MS", Helvetica, sans-serif;
}
fieldset {
  width: 380px;
  border: 2px solid #222;
  border-radius: 8px;
  margin: auto;
  min-height: 10px;
  line-height: 10px;
  position: relative;
  float: center;
  padding: 20px;
  top: 60px;
}
h2 {
  font-family: "Lobster", cursive;
  font-weight: normal;
  color: #bb86fc;
  text-shadow: 0 1px 0 rgba(255, 255, 255, 0.12);
  display: inline-block;
  margin: 0;
  line-height: 1.3;
  position: absolute;
  top: -20px;
  background: #1c1920;
  padding: 0 5px 0 5px;
}
.blur {
  filter: blur(20px);
}
input[type="radio"] {
  display: none;
} /* ------------------- Default State -------------------- */
input[type="radio"] + label {
  z-index: 10;
  margin: 20px 10px 10px 7px;
  width: 115.5px;
  height: 37.29px;
  line-height: 36.3px;
  position: relative;
  font-size: 13.2px;
  letter-spacing: 0.1em;
  color: #0e0e0e;
  text-shadow: 0 1px 0 rgba(255, 255, 255, 0.1);
  font-weight: bold;
  background-image: linear-gradient(#111, #000);
  border-radius: 3px;
  box-shadow: 0 -1px 0 rgba(0, 0, 0, 0.2), 0 0 0 1px rgba(0, 0, 0, 0.3),
    0 1px 0 rgba(255, 255, 255, 0.05), 0 0 3px rgba(255, 255, 255, 0.2);
}
input[type="radio"] + label:after {
  z-index: -1;
  content: "";
  cursor: pointer;
  top: 1.98px;
  margin-left: 50%;
  left: -55px;
  width: 110px;
  height: 33px;
  display: block;
  position: absolute;
  background-image: linear-gradient(#444, #373738);
  border-radius: 2px;
  box-shadow: inset 0 2px 1px -1px rgba(255, 255, 255, 0.2),
    inset 0 -2px 1px -1px rgba(0, 0, 0, 0.2), 0 12px 12px rgba(0, 0, 0, 0.5),
    0 4px 6px rgba(0, 0, 0, 0.3);
} /* ------------------- Hover States -------------------- */
a.hover,
input[type="radio"] + label:hover {
  color: #aeaeae;
  text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.4);
}
a.hover:after,
input[type="radio"] + label:hover:after {
  background-image: linear-gradient(#02aab0, #00cdac);
} /* ------------------- Active States -------------------- */
a.active,
a.active:hover,
input[type="radio"]:checked + label {
  color: #00ccff;
  text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.3), 0 0px 30px rgba(0, 255, 255, 0.7);
  box-shadow: 0 -1px 0 rgba(0, 193, 255, 0.2), 0 0 0 1px rgba(0, 0, 0, 0.4),
    0 1px 0 rgba(255, 255, 255, 0.05), 0 0 20px rgba(0, 193, 255, 0.1),
    0 0 30px rgba(0, 193, 255, 0.1);
}
a.active,
a.active:hover {
  text-shadow: 0 -1px 0 rgba(0, 0, 0, 1), 0 0px 30px rgba(0, 255, 255, 1),
    0 0px 50px rgba(0, 255, 255, 1);
}
a.active:after,
a.active:hover:after,
input[type="radio"]:checked + label:after {
  background-image: linear-gradient(#262627, #2d2d2e);
  box-shadow: inset 0 5px 6px rgba(0, 0, 0, 0.3),
    inset 0 0 4px rgba(0, 0, 0, 0.9), 0 0 0 black;
} /* ------------------- Radio Specific Style -------------------- */
input[type="radio"] + label {
  font-family: "ModernPictogramsNormal", Arial, sans-serif;
  font-size: 36.6666666667px;
  line-height: 55px;
  letter-spacing: 0;
  width: 55px;
  height: 55px;
}
input[type="radio"] + label:after {
  top: 2.64px;
  left: -25px;
  width: 50px;
  height: 50px;
}
.rainbow-text {
  background-image: linear-gradient(90deg, #e646ff, #8737ff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}
.center {
  position: relative;
  left: 10px;
  top: 10px;
}
input[type="checkbox"] {
  margin: 10px;
  position: relative;
  cursor: pointer;
  width: 150px;
  height: 55px;
  align: right;
  -webkit-appearance: none;
  background: linear-gradient(0deg, #333, #000);
  outline: none;
  border-radius: 40px;
  box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e,
    inset 0 0 10px rgba(0, 0, 0, 1);
}
input:checked[type="checkbox"]:nth-of-type(1) {
  background: linear-gradient(
    60deg,
    #f79533,
    #f37055,
    #ef4e7b,
    #a166ab,
    #5073b8,
    #1098ad,
    #07b39b,
    #6fba82
  );
  box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e,
    inset 0 0 10px rgba(0, 0, 0, 1);
}
input:checked[type="checkbox"]:nth-of-type(2) {
  background: linear-gradient(60deg, #ef32d9, #00f260, #0575e6);
  box-shadow: 0 0 0 4px #353535, 0 0 0 5px #3e3e3e,
    inset 0 0 10px rgba(0, 0, 0, 1);
}
input:checked[type="checkbox"]:nth-of-type(3) {
  background: linear-gradient(0deg, #70a1ff, #1e90ff);
  box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e,
    inset 0 0 10px rgba(0, 0, 0, 1);
}
input:checked[type="checkbox"]:nth-of-type(4) {
  background: linear-gradient(0deg, #e67e22, #f39c12);
  box-shadow: 0 0 0 4px #353535, 0 0 0 5px #3e3e3e,
    inset 0 0 10px rgba(0, 0, 0, 1);
}
input[type="checkbox"]:before {
  content: "";
  position: absolute;
  top: 0;
  left: 0px;
  width: 70px;
  height: 55px;
  background: linear-gradient(0deg, #000, #6b6b6b);
  border-radius: 40px;
  box-shadow: 0 0 0 1px #232323;
  transform: scale(0.98, 0.96);
  transition: 0.5s;
}
input:checked[type="checkbox"]:before {
  left: 80px;
} /*---DOT SEEN IN BEETWEEN-->*/
input[type="checkbox"]:after {
  content: "";
  position: absolute;
  top: calc(50% - 2px);
  left: 15px;
  width: 4px;
  height: 4px;
  background: linear-gradient(0deg, #6b6b6b, #000);
  border-radius: 50%;
  transition: 0.5s;
}
input:checked[type="checkbox"]:after {
  left: 130px;
}
h1 {
  margin: 20px, 0, 0, 40px;
  padding: 0;
  right: 1%;
  font-family: sans-serif;
  color: #fff;
  font-size: 20px;
  padding: 15px 0;
  text-transform: uppercase;
  letter-spacing: 4px;
  display: inline-block;
  position: absolute;
  align: right;
}
h5 {
  position: absolute;
  left: 50%;
  top: 30px;
  transform: translate(-50%, -50%);
  margin: 0;
  padding: 0;
  font-family: Lucidatypewriter, monospace;
  text-align: center;
  color: #fff;
  font-size: 35px; /*padding: 35px 0;*/
  text-transform: uppercase;
  letter-spacing: 4px;
  display: inline-block;
} 
/* -------------------  SuperBtn -------------------- */
a.superBtn {
  color: #0e0e0e;
  text-shadow: 0 1px 0 rgba(255, 255, 255, 0.1);
  font-weight: bold;
  letter-spacing: 0.08em;
  width: 242px;
  height: 39.6px;
  line-height: 39.6px;
  position: relative;
  border-radius: 30px;
  display: inline-block;
  background-image: linear-gradient(#2e3537, #424748);
  box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.6),
    inset 0 -1px 0 rgba(0, 0, 0, 0.3), inset 0 0 0px 1px rgba(0, 0, 0, 0.1),
    0 0 1px rgba(0, 0, 0, 0.5), 0 2px 2px rgba(0, 0, 0, 0.4);
}
a.superBtn:after {
  width: 253px;
  height: 49.5px;
  top: -4.95px;
  margin-left: 50%;
  left: -126.5px;
  content: "";
  display: block;
  border-radius: 30px;
  background-image: linear-gradient(#2af1fc, #17b2e6);
  box-shadow: 0 -1px 1px rgba(0, 0, 0, 0.7), 0 1px 1px rgba(255, 255, 255, 0.4),
    0 0 1px 1px rgba(0, 0, 0, 0.3), inset 0 0px 3px rgba(0, 0, 0, 0.4),
    inset 0 1px 2px rgba(0, 0, 0, 0.6);
  position: absolute;
  z-index: -1;
} /* ------------------- SuperBtn Hover -------------------- */
a.superBtn:hover {
  color: #aeaeae;
  text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.8);
  background-image: linear-gradient(#545454, #373738);
}
a.superBtn:hover:after {
  box-shadow: 0 -1px 1px rgba(0, 0, 0, 0.7), 0 1px 1px rgba(255, 255, 255, 0.4),
    0 0 1px 1px rgba(0, 0, 0, 0.3), inset 0 0px 3px rgba(0, 0, 0, 0.4),
    inset 0 1px 2px rgba(0, 0, 0, 0.6), 0 0 15px 5px rgba(0, 255, 255, 0.2);
}
a.superBtn:active {
  color: #0cf;
  text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.3), 0 0px 30px rgba(0, 255, 255, 0.7);
  background: #333;
  box-shadow: 0 1px 0px rgba(255, 255, 255, 0.4),
    inset 0 3px 3px rgba(0, 0, 0, 0.7), inset 0 0 4px rgba(0, 0, 0, 0.9),
    0 0 0 black;
}
h6
{
  display: inline-block;
    background-image: linear-gradient(#545454, #373738);
  padding:10px;
  cursor: pointer;
}

    </style>
  <body onload="">
  
  <h5 class='rainbow-text'>SMART HOME</h5>
<fieldset>
  <h2 class='blur'>Switches!</h2>
  <h2>Switches!</h2>
  <div class='center'>
    <h1> Light </h1> <input onClick='s1()' type='checkbox' name='checkbox1' id='checkbox1'> </br>
    <h1> FAN </h1> <input onClick='s2()' type='checkbox' name='checkbox2' id='checkbox2'> </br>
    <h1> NIGHT BULB </h1> <input onClick='s3()' type='checkbox' name='checkbox3' id='checkbox3'> </br>
    <h1> PLUG POINT </h1> <input onClick='s4()' type='checkbox' name='checkbox4' id='checkbox4'>
  </div>
</fieldset><br />
<fieldset>
  <h2 class='blur'>Fan Speed!</h2>
  <h2>Fan Speed!</h2> <input type='radio' id='radio1-1' onClick='s5(1)' checked='checked' name='radio'> <label for='radio1-1'>1</label> <input type='radio' id='radio1-2' onClick='s5(25)' name='radio'> <label for='radio1-2'>2</label> <input type='radio' id='radio1-3' onClick='s5(50)' name='radio'> <label for='radio1-3'>3</label> <input type='radio' id='radio1-4' onClick='s5(75)' name='radio'> <label for='radio1-4'>4</label> <input type='radio' id='radio1-5' onClick='s5(100)' name='radio'> <label for='radio1-5'>5</label>
</fieldset></br>
<fieldset>
   <h2 class='blur'>The Super Switch!</h2>
  <h2>The Super Switch!</h2>
   
  <center> <a onclick='s6()' class='superBtn'>ON/OFF All Switch</a> </center>
</fieldset>
<br/>
<br/>
<fieldset>
   <h2 class='blur'>Config</h2>
  <h2>Config</h2>
  <center> <a href='/conf' class='superBtn'>Configure</a> </center>
</fieldset>
</div>
  </body>

<SCRIPT>

    var data = "";
    var flg_chk1=0;
    var flg_unchk1=0;
    var flg_chk2=0;
    var flg_unchk2=0;
    var flg_chk3=0;
    var flg_unchk3=0;
    var flg_chk4=0;
    var flg_unchk4=0;
    var flg_chk5=0;
    var flg_unchk5=0;
    var s5val=0;
    var flag_load=0;
    
    InitWebSocket();

function InitWebSocket() 
{
  websock = new WebSocket("ws://" + window.location.hostname + ":88/");
  websock.onmessage = function (evt) 
  {
    JSONobj = JSON.parse(evt.data);
    s5val=JSONobj.sw5;
    if ( JSONobj.sw1 == " 1 ") 
    {
        if(flg_chk1==0)
        {
            document.getElementById('checkbox1').checked = true;
            flg_chk1=1;
            flg_unchk1=0;
        }
      
    } else if( JSONobj.sw1 == " 0 ")  
    {
        if(flg_unchk1==0)
        {
            document.getElementById("checkbox1").checked = false;
            flg_chk1=0;
            flg_unchk1=1;       
        }  
    }
    if (JSONobj.sw2 == " 1 ") 
    {
        if(flg_chk2==0)
        {
            document.getElementById("checkbox2").checked = true;
            flg_chk2=1;
            flg_unchk2=0;       
        }
      
    } else if (JSONobj.sw2 == " 0 ") 
    {
        if(flg_unchk2==0)
        {
            document.getElementById("checkbox2").checked = false;
            flg_unchk2=1;
            flg_chk2=0;     
        }  
    }
    if (JSONobj.sw3 == " 1 ") 
    {
        if(flg_chk3==0)
        {
            document.getElementById("checkbox3").checked = true;
            flg_chk3=1;
            flg_unchk3=0;       
        }
      
    } else if (JSONobj.sw3 == " 0 ")  
    {
        if(flg_unchk3==0)
        {
            document.getElementById("checkbox3").checked = false;
            flg_unchk3=1;
            flg_chk3=0;     
        }  
    }
    if (JSONobj.sw4 == " 1 ") 
    {
        if(flg_chk4==0)
        {
            document.getElementById("checkbox4").checked = true;
            flg_chk4=1;
            flg_unchk4=0;       
        }
      
    } else if(JSONobj.sw4 == " 0 ") 
    {
        if(flg_unchk4==0)
        {
            document.getElementById("checkbox4").checked = false;
            flg_unchk4=1;
            flg_chk4=0;     
        }  
    }
    

              if(103>s5val && s5val>80) 
              { 
                document.getElementById('radio1-5').checked = true; 
              } 
              else if(79>s5val && s5val>60)
                {
                 document.getElementById('radio1-4').checked = true; 
               }
               else if(59>s5val && s5val>40)
                {
                  document.getElementById('radio1-3').checked = true;
               }
               else if(39>s5val && s5val>20) 
               { 
                document.getElementById('radio1-2').checked = true;
               }
               else if(19>s5val && s5val>-1) 
               { 
                document.getElementById('radio1-1').checked = true; 
               }
  };
}



function s1() {
  btn = "sw1=ON";
  if (document.getElementById("checkbox1").checked == true) 
  {
    btn = "sw1=ON";
  
  }
  else
  {
    btn = "sw1=OFF";
  
  }
  websock.send(btn);
}

function s2() {
  btn = "sw2=ON";
  if (document.getElementById("checkbox2").checked == true) 
  {
    btn = "sw2=ON";
  
  }
  else
  {
    btn = "sw2=OFF";
  
  }
  websock.send(btn);
}
function s3() {
  btn = "sw3=ON";
  if (document.getElementById("checkbox3").checked == true) 
  {
    btn = "sw3=ON";
  
  }
  else
  {
    btn = "sw3=OFF";
  
  }
  websock.send(btn);
}

function s4() {
  btn = "sw4=ON";
  if (document.getElementById("checkbox4").checked == true) 
  {
    btn = "sw4=ON";
  
  }
  else
  {
    btn = "sw4=OFF";
  
  }
  websock.send(btn);
}
  function s5(fanval) {
        btn = "sw5="+fanval;
      websock.send(btn);
}

function s6() {
  btn = "sw4=ON";
  if (document.getElementById("checkbox1").checked == true || document.getElementById("checkbox2").checked == true || document.getElementById("checkbox3").checked == true || document.getElementById("checkbox4").checked == true) 
  {
    document.getElementById("checkbox1").checked = false;
    document.getElementById("checkbox2").checked = false;
    document.getElementById("checkbox3").checked = false;
    document.getElementById("checkbox4").checked = false;
    btn = "sw6=OFF";
  }
  else
  {
    document.getElementById("checkbox1").checked = true;
    document.getElementById("checkbox2").checked = true;
    document.getElementById("checkbox3").checked = true;
    document.getElementById("checkbox4").checked = true;
    
    btn = "sw6=ON";
  }
  websock.send(btn);
}

  </SCRIPT>
  </html>     
)=====";








const char* config_page = R"html(
<HTML>
<style>
body {
    width: 100%;
    margin: 0;
    padding: 0;
    background: #121212;
    padding: 30px;
}

fieldset {
    width: 380px;
    border: 2px solid #222;
    border-radius: 8px;
    margin: auto;
    min-height: 10px;
    line-height: 10px;
    position: relative;
    float: center;
    padding: 20px;
    top: 60px;
}

h2 {
    font-family: 'Lobster', cursive;
    font-weight: normal;
    color: #bb86fc;
    text-shadow: 0 1px 0 rgba(255, 255, 255, 0.12);
    display: inline-block;
    margin: 0;
    line-height: 1.3;
    position: absolute;
    top: -20px;
    background: #1c1920;
    padding: 0 5px 0 5px;
}

.blur {
    filter: blur(20px);
}

input[type='checkbox'] {
    padding-top: 18px;
    line-height: 15px;
    padding-bottom: 18px;
    text-align: center;
    color: #000;
    text-transform: uppercase;
    font-weight: 600;
    margin-left: 60px;
    margin-bottom: 0px;
    margin-top: 10px;
    cursor: pointer;
    display: inline-block;
    -webkit-appearance: none;
    outline: none;
    box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

.button {
    width: 100px;
    padding-top: 18px;
    line-height: 15px;
    padding-bottom: 18px;
    text-align: center;
    color: #000;
    text-transform: uppercase;
    font-weight: 600;
    margin-left: 60px;
    margin-bottom: 0px;
    margin-top: 10px;
    cursor: pointer;
    display: inline-block;
}

.button-1 {
    background-color: transparent;
    border: 2px solid #2af1fc;
    border-radius: 30px;
    -webkit-transition: all 0.15s ease-in-out;
    transition: all 0.15s ease-in-out;
    color: #00d7c3;
}

.button-1:hover {
    box-shadow: 0 0 10px 0 #00d7c3 inset, 0 0 20px 2px #00d7c3;
    border: 3px solid #00d7c3;
}
</style>
<script>
function reset() { var r = confirm('Do You want to Reset the device??!!All Data will be deleted.'); if (r == true) { document.getElementById('rst').submit(); } else { alert('Cancel!!!'); } }

function reboot() { var r = confirm('Do You want to Reboot the device?'); if (r == true) { document.getElementById('rbt').submit(); } else { alert('Cancel!!!'); } }

function alertbox() { alert('Working on it!!'); }
</script>

<BODY>
    <fieldset> <a href='/' class='button button-1'> Back </a> <!--<a href='/xml' target='_blank' class='button button-1'> XML </a>--></fieldset></br></br></br>
    <fieldset>
        <h2 class='blur'>MODE!</h2>
        <h2>MODE!!!</h2>
        <div onclick='alertbox()' class='button button-1'>Touch Switch</div>
        <div onclick='alertbox()' class='button button-1'>Press Switch</div>
        <div onclick='alertbox()' class='button button-1'>NORMAL Switch</div>
    </fieldset></br></br>
    <fieldset>
        <h2 class='blur'>SYSTEM!</h2>
        <h2>SYSTEM!!!</h2>
        <div onclick='reset()' class='button button-1'>Reset</div>
        <div onclick='reboot()' class='button button-1'> Reboot </div>
    </fieldset>
    <form id='rst' method='get' action='reset'></form>
    <form id='rbt' method='get' action='reboot'></form>
</BODY>
</HTML>
)html";




void handleWebsite()
{
  server.send(200,"text/html",webSite);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Action function is giving output to nodemce(Controlling Relay) 

void action()
{
  digitalWrite(OutPin1, OutPin1State);
  digitalWrite(OutPin2, OutPin2State);
  digitalWrite(OutPin3, OutPin3State);
  digitalWrite(OutPin4, OutPin4State);
  
  if(chk_valforslider!=valueforslider || chk_valforslider==200)
  { Serial.println(valueforslider); }
  // Updating BLYNK APP 
  Blynk.virtualWrite(V3,  OutPin1State);  //Update button widget 
  Blynk.virtualWrite(V4,  OutPin2State);  //Update button widget 
  Blynk.virtualWrite(V5,  OutPin3State);  //Update button widget 
  Blynk.virtualWrite(V6,  OutPin4State);  //Update button widget 
  Blynk.virtualWrite(V10, valueforslider);  //Update button widget
}


//WebsocketEvent is response coming from website

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t wslength)
{
         if(type==WStype_CONNECTED)
            {
                pageonload=1;
            }
    String payloadString=(const char *)payload;
  //Serial.println(payloadString);
  //Serial.println("payload: '"+payloadString+"', channel: "+(String)num);
    if(type==WStype_TEXT)
      {
          byte separator=payloadString.indexOf('=');
          String var=payloadString.substring(0,separator);
          String val=payloadString.substring(separator+1);
          
          // Serial.println(var);
          // Serial.println(val);
          if(var == "sw1") {if(val=="ON"){OutPin1State=1;}else{OutPin1State=0;}}
          if(var == "sw2") {if(val=="ON"){OutPin2State=1;}else{OutPin2State=0;}}
          if(var == "sw3") {if(val=="ON"){OutPin3State=1;}else{OutPin3State=0;}}
          if(var == "sw4") {if(val=="ON"){OutPin4State=1;}else{OutPin4State=0;}}
          if(var == "sw5") {valueforslider = val.toInt();}
          if(var == "sw6") {if(val=="ON"){OutPin1State=1;OutPin2State=1;OutPin3State=1;OutPin4State=1; pageonload=1;}
                            else{OutPin1State=0;OutPin2State=0;OutPin3State=0;OutPin4State=0;pageonload=1;}}
          
        }
}
         
    
    
          /////// FUNCTION FOR PHYSICAL BUTTON CHECK
void check_normal_Btn()
    {
      //Switch 1
      
      if (digitalRead(btnPin1) == LOW)
      {
        if(btnPin1old==0 || btnPin1old==2) 
        { OutPin1State=HIGH; jsonprint=1;JSONtxt="{\"sw1\":\" "+(String)OutPin1State+" \"}";
          btnPin1old=1; }
      }else
      {
         if(btnPin1old==1 || btnPin1old==2) 
         {OutPin1State=LOW; jsonprint=1;JSONtxt="{\"sw1\":\" "+(String)OutPin1State+" \"}";
          btnPin1old=0;}
      }
      
      //Switch 2
      
      if (digitalRead(btnPin2) == LOW)
      {
        if(btnPin2old==0 || btnPin2old==2) 
        { OutPin2State=HIGH; jsonprint=1;JSONtxt="{\"sw2\":\" "+(String)OutPin2State+" \"}";
          btnPin2old=1; }
      }else
      {
         if(btnPin2old==1 || btnPin2old==2) 
         { OutPin2State=LOW; jsonprint=1;JSONtxt="{\"sw2\":\" "+(String)OutPin2State+" \"}";
         btnPin2old=0; }
      }
      
      //Switch 3
      
      if (digitalRead(btnPin3) == LOW)
      {
        if(btnPin3old==0 || btnPin3old==2) 
        { OutPin3State=HIGH; jsonprint=1;JSONtxt="{\"sw3\":\" "+(String)OutPin3State+" \"}";
          btnPin3old=1; }
      }else
      {
         if(btnPin3old==1 || btnPin3old==2) 
         { OutPin3State=LOW; jsonprint=1;JSONtxt="{\"sw3\":\" "+(String)OutPin3State+" \"}";
         btnPin3old=0; }
      }
      
      //Switch 4
      
      if (digitalRead(btnPin4) == LOW)
      {
        if(btnPin4old==0 || btnPin4old==2) 
        { OutPin4State=HIGH; jsonprint=1;JSONtxt="{\"sw4\":\" "+(String)OutPin4State+" \"}";
          btnPin4old=1; }
      }else
      {
         if(btnPin4old==1 || btnPin4old==2) 
         { OutPin4State=LOW; jsonprint=1;JSONtxt="{\"sw4\":\" "+(String)OutPin4State+" \"}";
         btnPin4old=0; }
      }
    }

          
          void checkPressButton()
          {
                  if (digitalRead(btnPin1) == LOW){ 
                     if (btnPin1State != LOW){OutPin1State = !OutPin1State;}btnPin1State = LOW; jsonprint=1;JSONtxt="{\"sw1\":\" "+(String)OutPin1State+" \"}";}
                  else{btnPin1State = HIGH;}
                  ////for button 2
                  if (digitalRead(btnPin2) == LOW){
                     if (btnPin2State != LOW){OutPin2State = !OutPin2State;}btnPin2State = LOW; jsonprint=1;JSONtxt="{\"sw2\":\" "+(String)OutPin2State+" \"}";}
                  else{ btnPin2State = HIGH; }
                  ////for button 3
                  if (digitalRead(btnPin3) == LOW){
                     if (btnPin3State != LOW){OutPin3State = !OutPin3State;}btnPin3State = LOW; jsonprint=1;JSONtxt="{\"sw3\":\" "+(String)OutPin3State+" \"}";}
                  else{ btnPin3State = HIGH; }
                  ////for button 4
                  if (digitalRead(btnPin4) == LOW){
                     if (btnPin4State != LOW){OutPin4State = !OutPin4State;}btnPin4State = LOW; jsonprint=1;JSONtxt="{\"sw4\":\" "+(String)OutPin4State+" \"}";}
                  else{ btnPin4State = HIGH; }
          }

            void checkPhysicalButton()
            {
               if (String(configStore.switchtype)=="normal")
               {
                 // Setup a function to be called every 100 ms it will check the hard switch action.              
                timer.setInterval(100L, check_normal_Btn);
                Serial.println("Normal Switch is Running Now.... ");
                
               }else if(String(configStore.switchtype)=="press")
               {
                  // Setup a function to be called every 100 ms it will check the hard switch action.              
                  timer.setInterval(100L, checkPressButton);
                  Serial.println("Press Switch is Running Now....");
               }
               else 
               {
                  // Setup a function to be called every 100 ms it will check the hard switch action.              
                  timer.setInterval(100L, checkPressButton);
                 Serial.println("Switch is not selected so running Default Press switch....");
               }


            }
///////////////////////////////////////////////////////////////////////////////////////////////////

//==========================================SETUP======================================
void setup() {
  delay(500);
  Serial.begin(115200);
  
 
  BlynkProvisioning.begin();    
  
  // == I/O PIN INTIALISATION
      
              pinMode(OutPin1, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin1, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin2, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin2, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin3, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin3, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin4, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin4, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
    //== I/O pin END              
 
}


//==================================     LOOP   ==================================================
void loop() {
  // This handles the network and cloud connection
   BlynkProvisioning.run();
   webSocket.loop();
 
   if(first_time==1)
   {
        if(WiFi.status() != WL_CONNECTED)
        {//if wifi is not connected then retry connecting
          if(ipprint==1)
          {
              Serial.println("Wifi Disconnected. Retrying to connect to wifi if possible !!!");
              ipprint=0;
              server.stop();
          }
        }else
        {//if wifi is connected
        // Coding for WEBserver start and handling
             if(ipprint==0) // ipprint save the value to print on serial only first time.
                {
                    server.begin();
                    webSocket.begin();
                    webSocket.onEvent(webSocketEvent);

                    server.on("/", []() {
                        server.send(200, "text/html", webSite);
                    });
                    server.on("/conf", []() {
                        server.send(200, "text/html", config_page);
                    });
            
                    server.on("/reset", []() {
                        Serial.println("Resetting Device...");
                        enterResetConfig();
                    });
                    server.on("/reboot", []() {
                        Serial.println("Rebooting Device...");
                        restartMCU();
                    });
            
                    Serial.println(WiFi.localIP());
                    ipprint=1;
             //====================== Starting DNS SERVER======================  
                    
                    dnServer.setTTL(300); // Time-to-live 300s
                    dnServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
                    dnServer.start(DNS_PORT, BOARD_CONFIG_AP_URL, WiFi.localIP());
                    server.onNotFound([]() {
                           String message = "Hello World!\n\n";
                                 message += "URI: ";
                                 message += server.uri();
                            server.send(200, "text/plain", message);
                    });
  
                }

            if(pageonload==1)
            {
                JSONtxt="{\"sw1\":\" "+(String)OutPin1State+" \",\"sw2\":\" "+(String)OutPin2State+" \",\"sw3\":\" "+(String)OutPin3State+" \",\"sw4\":\" "+(String)OutPin4State+" \",\"sw5\":\" "+(String)valueforslider+" \"}";
                webSocket.broadcastTXT(JSONtxt);
                pageonload=0;   
            }
             if(jsonprint==1)
            {

                webSocket.broadcastTXT(JSONtxt);
                jsonprint=0;
            }      
            server.handleClient();
            dnServer.processNextRequest();
        }
       // Run function after first time and only one time.
       if (switch_flag==0)
       {checkPhysicalButton();switch_flag=1;}
       timer.run();
       if(d1 != OutPin1State ||d2 != OutPin2State ||d3 != OutPin3State ||d4 != OutPin4State ||d5 != valueforslider  )
       { 
        action();
       }

    }  
  ////////////////Coding in loop for accepting values from Arduino Fan speed
     
                    
           if (Serial.available() > 0 ) 
            {
              rdata = Serial.read(); 
              myString = myString+ rdata; 
             // Serial.print(rdata);
              if( rdata == '\n')
              {
             //Serial.println(myString);
                valueforslider = myString.toInt();
                myString = "";
                Blynk.virtualWrite(V10, valueforslider);
                jsonprint=1;
                JSONtxt="{\"sw5\":\" "+(String)valueforslider+" \"}";
              // end new code
              }
            }


  /////////////////////////////////////////////////////////////////////////     

  //Serial.println("sudhir1 :" +sudhir1);
  //Serial.println("start :" +start);
  
  d1 =OutPin1State;
  d2 =OutPin2State;    
  d3 =OutPin3State;
  d4 =OutPin4State;
  d5 =valueforslider;
  chk_valforslider=valueforslider;

}

///////////////////////////////////////=========END OF LOOP=======================///////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//BLYNK FUNCTION TO CONTROL GPIO  PIN
        
        // You can send commands from Terminal to your hardware. Just use
        // the same Virtual Pin as your Terminal Widget
    
        BLYNK_WRITE(V1)//v1 set to control Terminal Print.
        {
      terminal.clear();
      // if you type "IP" into Terminal Widget - it will respond: "IP ADDRESS:"
      if (String("ip") == param.asStr()||String("IP") == param.asStr()) 
      {
              terminal.println("IP Address : ") ;
        terminal.println(WiFi.localIP());
      } else 
      {
        terminal.print("IP : ");
        terminal.println(WiFi.localIP());
        terminal.println("if you type 'IP' into Terminal Widget - it will respond: 'IP ADDRESS:'");
            }
          // Ensure everything is sent
          terminal.flush();
        }
          // When App button is pushed - switch the state
          // Map this Virtual Pin to your Mobile Blynk apps widget.
          
          BLYNK_WRITE(V3)
          {                           
             OutPin1State = param.asInt();
             digitalWrite(OutPin1, OutPin1State);
             jsonprint=1;
             JSONtxt="{\"sw1\":\" "+(String)OutPin1State+" \"}";
          }
      BLYNK_WRITE(V4)
          {                           
             OutPin2State = param.asInt();
             digitalWrite(OutPin2, OutPin2State);
             jsonprint=1;
             JSONtxt="{\"sw2\":\" "+(String)OutPin2State+" \"}";
          }
          BLYNK_WRITE(V5)
          {                           
             OutPin3State = param.asInt();
             digitalWrite(OutPin3, OutPin3State);
             jsonprint=1;
             JSONtxt="{\"sw3\":\" "+(String)OutPin3State+" \"}";
          }
          BLYNK_WRITE(V6)
          {                           
             OutPin4State = param.asInt();
             digitalWrite(OutPin4, OutPin4State);
             jsonprint=1;
             JSONtxt="{\"sw4\":\" "+(String)OutPin4State+" \"}";
          }
      
    
      BLYNK_WRITE(V10) // send Fan values to arduino to set fan speed
       {
         valueforslider = param.asInt(); // assigning incoming value from pin V10 to a variable
          if (chk_valforslider!=valueforslider || chk_valforslider==200)
        {
          Serial.println(valueforslider);
           jsonprint=1;
           JSONtxt="{\"sw5\":\" "+(String)valueforslider+" \"}";
        }
        delay(100); 
       }
 
  // Every time we connect to the cloud...
          BLYNK_CONNECTED()
          {
            // Request the latest state from the server
            
               Blynk.syncVirtual(V1);
               Blynk.syncVirtual(V3);
               Blynk.syncVirtual(V4);
               Blynk.syncVirtual(V5);
               Blynk.syncVirtual(V6);
               Blynk.syncVirtual(V10);
          }
          
