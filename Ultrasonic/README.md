Grove_Ultrasonic_Ranger
--------------------------------

[![Grove_Ultrasonic_Ranger](http://www.seeedstudio.com/wiki/images/thumb/3/3a/Ultrasonic_Ranger.jpg/300px-Ultrasonic_Ranger.jpg)](http://www.seeedstudio.com/depot/grove-ultrasonic-ranger-p-960.html?cPath=144_149)

This Grove - ultrasonic sensor is a non-contact distance measurement module which works at 42KHz, suitable for projects that require middle distance measurement. 

**Note:**

This library is used for Seeed graphical programming Project only.


**Library Name**：Grove_Ultrasonic_Ranger 

**API**
---

	/* 
	 * Function Name: UltrasonicRangerRead 
	 * Input: PinName, pin to connect 
	 * Return: distance in cm   
	 */ 
	int UltrasonicRangerRead(int PinName); 

Test Case 
---

	Get data from then sensor per 100ms, and print to Serial monitor.   
	// test for seeed graphical programming project 
	// loovee @ 2015-8-4 
	 
	#include "Ultrasonic.h" 
	 
	Ultrasonic ultrasonic; 
	const int pin = 2;          // ultrasonic connect to D2 
	 
	void setup() 
	{ 
	    Serial.begin(115200); 
	    Serial.println("test for Seeed Graphical programming project"); 
	    Serial.println("Ultrasonic"); 
	} 
	 
	void loop() 
	{ 
	    int dist_cm = ultrasonic.UltrasonicRangerRead(pin);         // read data from ultrasonic ranger 
	    Serial.println(dist_cm);                                    // display it 
	    delay(100); 
	}


    
----

This software is written by Loovee for for [Seeed Technology Inc.](http://www.seeed.cc) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt/LICENSE for the details of MIT license.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.


[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Grove_Ultrasonic_Ranger)](https://github.com/igrigorik/ga-beacon)


