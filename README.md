Pet-Water-Warden
================

Here's an automatic minder to keep your furry friends' water supply topped up while you're away or indisposed. A simple homemade dip probe, connected to a microcontroller, senses the water level in a pet's dish and automatically activates a pump to refill it from a reservoir when it gets low, or send a tweet when the reservoir runs out.

The system I built is based on an Arduino microcontroller, which reads three sensors and controls the waterworks. Two simple dip probes are built from just a transistor connected to some wire. The reservoir can be any size but I chose a 3-gallon jug — it lasts a good while but it's not too bulky. The pet water bowl is filled by a small aquarium pump that sits at the bottom of the reservoir. The tweeting capabilities come from an Arduino Wi-Fi shield, or an Ethernet shield if your router is close enough to your Water Warden (with some modifications to the code). If the pet bowl doesn't fill up after a set time in the code, due to an empty reservoir or a sensor failure, it will automatically shut off and post an error tweet, so you can hurry home and top off your pet's tank.

http://makezine.com/projects/petwaterwarden/
