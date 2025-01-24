@mainpage 34338-Project at DTU: Smart-home
This report is made in accordance with the course “34338: Telecommunication Programming
Projects with Arduino”. The goal of this project is to develop a smart home system, that
in some way, shape or form can gather and log data, and react upon it.

In this project, a smart-home access and monitoring system will be designed. The system
is created in two parts, one at the door and one inside the home, which can communicate via.
ThingSpeak and a webserver. At the door, a person scans their RFID chip, and if it matches a
user in the system, the door opens and the door system sends a message to ThingSpeak that
someone is now home. This causes an LCD at the door to show a welcome message.

The home environment monitoring system measures temperature and humidity in the home,
and always sends this data to ThingSpeak. If someone is home, it opens a window if the
temperature is above a certain threshold and closes it again when it falls below. The home
monitoring system also has an alarm system triggered by a motion sensor. If no one is home
and movement is detected, then the alarm will go off. The alarm can be turned off with a
button. The door system also hosts a webserver with a button to remotely open the door.

The project can be found on GitHub: https://github.com/liou0006/34883-Project
