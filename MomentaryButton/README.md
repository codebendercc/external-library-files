<style type="text/css">
<!--
.prosign {
	text-decoration: overline;
}
[title] {
	cursor: help;
	border-bottom: 1px dotted black;
}
img, a img {
	border: none;
}
#license {
	float: left;
	margin-right: 1ex;
}
-->
</style>

<h1>Bicycle Computer for Arduino LilyPad</h1>

<p>This is a bicycle computer, designed to count wheel revolutions using a <a href="http://en.wikipedia.org/wiki/Cyclocomputer#The_wheel_sensor">cyclocomputer wheel sensor</a>. It stores two trip meters and a speed histogram; the trip meters are reported in <a href="http://en.wikipedia.org/wiki/Morse_code">Morse code</a> via a speaker, and the histogram can be saved to <acronym title="Electrically Erasable Programmable Read-Only Memory"><a href="http://en.wikipedia.org/wiki/EEPROM">EEPROM</a></acronym> and later printed over the serial connection.</p>

<p>(Mouse over <span title="Morse definitions, acronym expansions, etc">text like this</span> for elucidatory tooltips.)</p>

<p>See <a href="http://www.markfickett.com/stuff/artPage.php?id=371">photos and a video</a> along with discussion concerning the hardware construction (sewing) and use.</p>

<h2>Authorship and License</h2>

<a rel="license" href="http://creativecommons.org/licenses/by/3.0/"><img alt="Creative Commons License" id="license" src="http://i.creativecommons.org/l/by/3.0/80x15.png" /></a>

<p>This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/3.0/">Creative Commons Attribution 3.0 Unported License</a>; it is written by <a href="http://www.markfickett.com/">Mark Fickett</a>. (Do <a href="mailto:mark.fickett@gmail.com">let me know</a> if you find it interesting or useful! Or, should you find bugs, a report or a <a href="http://help.github.com/pull-requests/">pull request</a> would be welcome.)</p>

<h2>Hardware</h2>

<p>The interface was designed for the LilyPad Arduino ATmega328 (<a href="http://web.media.mit.edu/~leah/LilyPad/">Leah Buechley</a>, <a href="http://arduino.cc/en/Main/ArduinoBoardLilyPad">arduino.cc</a>). All peripheral hardware has one lead connected to the specified pin, the other to ground.</p>

<ul>
<li><code>PIN_REV_SENSOR</code> (12) &mdash; connected to <a href="http://www.google.com/search?q=Vetta+C-15+cyclocomputer">a commercial cyclocomputer</a>'s sensor (chosen only because it was already on the bike)</li>
<li><code>PIN_SPEAKER</code> (3) &mdash; a buzzer, originally the ringer on a cordless touch-tone phone</li>
<li><code>PIN_BUTTON_REPORT</code> (2) &mdash; a momentary switch, easily triggered without looking (<a href="http://www.sparkfun.com/products/8605">similar on sparkfun</a>), originally from a <acronym>VCR</acronym>'s circuit board</li>
<li><code>PIN_BUTTON_A</code> and <code>PIN_BUTTON_B</code> (11 and 12) &mdash; <a href="http://www.sparkfun.com/products/8776">LilyPad Button Board</a>s, difficult to trigger accidentally</li>
<li>and <code>PIN_STATUS</code> (13) &mdash; the built-in status <acronym>LED</acronym></li>
</ul>

<h2>Controls and Outputs</h2>

<p>The status light blinks <code>K</code> (dah-dit-dah, -.-) when <code>setup</code> completes, then a short blink each time the wheel sensor is triggered. The trip meters are named (in Morse output) <code>dit</code> or <code>dit-dit</code>. Button inputs are either a tap (quick press and release), or a hold:</p>

<ul>
<li>Tap <code>PIN_BUTTON_REPORT</code> to hear the trip meters' values in Morse on <code>PIN_SPEAKER</code>.</li>
<li>Tap <code>PIN_BUTTON_A</code> or <code>PIN_BUTTON_B</code> to reset the associated trip meter; the name of the trip meter and <code class="prosign" title="dit-dit-dah-dit-dah"><a href="http://en.wikipedia.org/wiki/Prosigns_for_Morse_code">SK</a></code> are sent as confirmation.</li>
</ul>
<ul>
<li>Hold <code>PIN_BUTTON_REPORT</code> to write the histogram values to serial output, formatted for <a href="http://www.r-project.org/" title="LaTeX meets MATLAB for statistics and graphing">R</a> (see <code>makeGraph.R</code>).</li>
<li>Hold <code>PIN_BUTTON_A</code> to save the histogram to <acronym>EEPROM</acronym>. Before writing <code title="dit-dit-dit">S</code> is sent; then the status <acronym>LED</acronym> is lit during writing, and finally a <code>dit</code> is sent after writing is complete. The histogram is always read from <acronym>EEPROM</acronym> on startup.</li>
<li>Hold <code>PIN_BUTTON_B</code> to clear the histogram. (This does not write the cleared histogram to <acronym>EEPROM</code>.) An <code title="dah-dit-dit-dah">X</code> is sent as confirmation.</li>
</ul>

<h2>Dependency: Morse Library</h2>

<p>The Morse code output uses <a href="http://github.com/markfickett/arduinomorse">a Morse library for Arduino</a>, the key feature of which is to allow monitoring an input (<code>PIN_REV_SENSOR</code>) while sending Morse.</p>
