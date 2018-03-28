/*

  " HELLO DRUM LIBRARY" Ver.0.1

  Created March 1, 2018
  by Ryo Kosaka

  https://open-e-drums.tumblr.com/
*/

#include "hellodrum.h"
#include "Arduino.h"


HelloDrum::HelloDrum(int pin1, int pin2)
{
  pin_1 = pin1;
  pin_2 = pin2;
}

HelloDrum::HelloDrum(int pin1)
{
  pin_1 = pin1;
}

/////////////////////////////////////////////////////////////////////////

void HelloDrum::singlePiezo(int sens, int thre1, int thre2, int retri) {

  int velo = 0;
  hit = false;
  piezoValue = analogRead(pin_1);

  if (analogRead(pin_1) > thre1) {
    time_hit = millis();

    if (time_hit - time_end < retri) {
      flag = true;
    }

    //        if (piezoValue - exValue > thre2) {
    //          flag = false;
    //        }

    if (flag == false) {

      for (int i = 0; i < 5; i++) {
        int peak = analogRead(pin_1);
        if (peak > velo) {
          velo = peak;
        }
        while (millis() - time_hit < 1);
      }

      velo = map(velo, thre1, sens, 1, 127);

      if (velo <= 1) {
        velo = 1;
      }

      if (velo > 127) {
        velo = 127;
      }

      //velo = (velo * velo) / 126 + 1;

      //      Serial.print(pin_1);
      //      Serial.print(", ");
      //      Serial.println(velo);

      velocity = velo;
      flag = true;
      hit = true;
    }
  }

  if (flag == true) {
    time_end = millis();
    exValue = analogRead(pin_1);
    flag = false;
  }
}

///////////////////////////////////////////////////////////////////////////////////////

//void HelloDrum::dualPiezo(void) {
//
//}
//

///////////////////////////////////////////////////////////////////////////////////////

void HelloDrum::cymbal3zone(int sens, int thre1, int thre2, int retri) {

  int velo = 0;
  hit = false;
  hit_edge = false;
  hit_cup = false;
  choke = false;

  int piezoValue = analogRead(pin_1);
  int sensorValue = analogRead(pin_2);

  //入り口でエッヂとボウ分けるか？
  //  if (abs(analogRead(pin_1) - analogRead(pin_2)) > thre1) {
  if (analogRead(pin_2) > thre1 || analogRead(pin_1) > thre1) {
    time_hit = millis();

    if (time_hit - time_end < retri) { //retrigger cancel
      flag = true;
    }

    //    if (analogRead(pin_1) - exValue > thre2) {
    //      flag = false;
    //    }

    if (flag == false) {
      for (int i = 0; i < 5; i++) {  //peak scan
        int peak = analogRead(pin_1);
        if (peak > velo) {
          velo = peak;
        }
        while (millis() - time_hit < 1);
      }

      //bow
      if (sensorValue < 50) {
        velo = map(velo, thre1, sens , 1, 127);

        if (velo <= 1) {
          velo = 1;
        }

        if (velo > 127) {
          velo = 127;
        }

        velocity = velo;
        flag = true;
        hit = true;
      }

      //edge
      else if (sensorValue > 500 && sensorValue < 520) {
        if (velo > 500) {
          velo = velo - 500;
        }
        else {
          velo = 500 - velo;
        }
        velo = map(velo, thre1, sens, 1, 127);

        if (velo <= 1) {
          velo = 1;
        }

        if (velo > 127) {
          velo = 127;
        }

        velocity = velo;
        hit_edge = true;
        flag = true;
      }

      //cup
      else if (sensorValue > 520) {
        //MIDI.sendNoteOn(53, 100, 10);   //(note, velocity, channel) cup note is 53
        //MIDI.sendNoteOff(53, 0, 10);
        velocity = 100;
        hit_cup = true;
        flag = true;
      }
    }
  }

  if (flag == true && piezoValue < thre1 && sensorValue < 10) {
    time_end = millis();
    flag = false;
  }

  if (chokeFlag == false && sensorValue > 500 && sensorValue < 900  && abs(piezoValue - sensorValue) < 10) {
    time_choke = millis();
    if (time_choke - time_end < 10) {
      chokeFlag = true;

    }
    else {
      choke = true;
      chokeFlag = true;
    }
  }

  if (flag == true &&  sensorValue < 10) {
    time_end = millis();
    chokeFlag = false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

void HelloDrum::cymbal2zone(int sens, int thre1, int thre2, int retri) {

  int velo = 0;
  hit = false;
  hit_edge = false;
  choke = false;
  int piezoValue = analogRead(pin_1);
  int sensorValue = analogRead(pin_2);

  //入り口でエッヂとボウ分けるか？
  //  if (abs(analogRead(pin_1) - analogRead(pin_2)) > thre1) {
  if (analogRead(pin_2) > thre1 || analogRead(pin_1) > thre1) {
    time_hit = millis();

    if (time_hit - time_end < retri) { //retrigger cancel
      flag = true;
    }

    //    if (analogRead(pin_1) - exValue > thre2) {
    //      flag = false;
    //    }

    if (flag == false) {
      for (int i = 0; i < 5; i++) {  //peak scan
        int peak = analogRead(pin_1);
        if (peak > velo) {
          velo = peak;
        }
        while (millis() - time_hit < 1);
      }

      //bow
      if (sensorValue < 50) {
        velo = map(velo, thre1, sens , 1, 127);

        if (velo <= 1) {
          velo = 1;
        }

        if (velo > 127) {
          velo = 127;
        }

        velocity = velo;
        flag = true;
        hit = true;
      }

      //edge
      else if (sensorValue > 500) {
        if (velo > 500) {
          velo = velo - 500;
        }
        else {
          velo = 500 - velo;
        }
        velo = map(velo, thre1, sens, 1, 127);

        if (velo <= 1) {
          velo = 1;
        }

        if (velo > 127) {
          velo = 127;
        }

        velocity = velo;
        flag = true;
        hit_edge = true;
      }
    }
  }

  if (flag == true && piezoValue < thre1 && sensorValue < 10) {
    time_end = millis();
    flag = false;
  }

  if (chokeFlag == false && sensorValue > 500 && sensorValue < 900  && abs(piezoValue - sensorValue) < 10) {
    time_choke = millis();
    if (time_choke - time_end < 10) {
      chokeFlag = true;

    }
    else {
      choke = true;
      chokeFlag = true;
    }
  }

  if (flag == true &&  sensorValue < 10) {
    time_end = millis();
    chokeFlag = false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void HelloDrum::TCRT5000(int sens, int thre1, int thre2) {
  
  int velo = 0;
  openHH = false;
  closeHH = false;
  int TCRT = analogRead(pin_1);

  if (TCRT < thre1 && closeHH == false && pedalVelocityFlag == false) {
    time_hit_pedal_1 = millis();
    pedalVelocityFlag = true;
  }

  if (TCRT < thre2 && closeHH == false) {
    time_hit_pedal_2 = millis();

    velo = time_hit_pedal_2 - time_hit_pedal_1;
    velo = map(velo, 50, 0, 1, 127);

    if (velo <= 1) {
      velo = 1;
    }

    if (velo > 127) {
      velo = 127;
    }

    velocity = velo;
    closeHH = true;
    openHH = false;
    
    pedalVelocityFlag = false;
  }


  if (TCRT > thre1 && closeHH == true) {
    closeHH = false;
    openHH = true;   
  }

  /////////////////////// HIHAT PEDAL CC

  TCRT = map(TCRT, 900, 100, 0, 127);
  if (TCRT > 127) {
    TCRT = 127;
  }
  if (TCRT < 0) {
    TCRT = 0;
  }

  if (exTCRT != TCRT) {
    pedalCC = TCRT;
    moving = true;
    exTCRT = TCRT;
  }

  else{
   moving = false;
  }

}

//////////////////////////////////////////////////////////////////
//void HelloDrum::FSR(void) {
//
//}
