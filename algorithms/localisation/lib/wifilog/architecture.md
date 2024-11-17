okay, what will this directory do?

basically it will provide remote debugging capabilities for arduino. This library will implement
a lightweight interface for arduino to talk to provided debugger using a custom-made protocol

This library will provide such abilities as:
- send serial data (for remote logging)
- send points data (for building maps of the surroundings)
- send state data from sensors and actuators
- send any other custom data

In addition to this thing a remote debugging client will also be created
it will probably be a webapplication (because no way I'm learning Qt for this one)
application will consist of modules and screens. Modules will process assigned datasteam,
and update the visuals of the frame. There will also probably be an ability to save 
configurations of the screens so it would be easy to work with the same robot without needing
to reconfigure the client each time.

This library will have a really modular structure, as to compliment the same structure used in the client.
There will be some base for establishing the connection, and data transmission, but not much from that.
Being modular will also make this library lightweight as uneeded modules will not get compiled and put on 
the microcontroller.

