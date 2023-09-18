# ITI-Graduation-Project
This is my project for ITI summer training (162 hours) . It is an auto parking Garage using Atmega32.
* SMART_Parking.c
 *this project is about a parking system 
 The admin will use the UART for entering the IDS of the users who has the right to enter the garage.
 then , the user will enter his personal id in the UART VT and if it is already stored by the admin , a green led will be high
 and a motor will open to open the door and the pir sensor will read 1 if the car entered and then it will close the motor of the door
 and SPI will send the id to another Micro "slave" to print on LCD the id of The user .
 if the user enterd wrong id , a red led will be high 
 if the user wants to go out from the garage , he will enter id to another door using UART VT
  and if it is right, the door will be open and so on..
