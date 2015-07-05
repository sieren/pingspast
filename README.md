PINGS FROM THE PAST
===================

**PINGS FROM THE PAST** is a sound-art installation made for the Sound And Masterials course by Dr. Caleb Kelly (UNSW) at **TU BERLIN** in 2015.

Modern mobile devices of all kinds constantly emit electro magnetic fields when looking for wireless networks they have once been connected to. This installation transforms these probe requests into sounds by analysing characteristics like signal strength, device vendor and the name of the wanted network.
The speaker has been modified to resonant with particles on top of it, in order to sonify the strength of the impact of the waves.

It is based on a Raspberry Pi, a monitor-mode capable WiFi-USB-Stick and a modified, amplified speaker.

The synth part is set to 8 voices. The probe-requests data structure (MAC Adresse, SNR, Probed AP) are translated to control the oscillators:<br />
MAC Address : First 4 HEX digits used for frequency (they identify the device vendor)<br
\>
SNR : Currently unused (Volume set statically to 0.5)<br \>
Probed AP : Length (Sine) or 2 seconds (Saw) if generic broadcast/no AP specified.<br \>

The final signal is then low-pass filtered to cut off the unpleasant, and in a installation venue rather reverberant high-frequency parts caused by the saw-oscillator.

**PROTOTYPE**
![Prototype installation](https://github.com/sieren/pingspast/blob/master/media/installationproto.jpg?raw=true)

-------------

Install
-------------

**Prequisites**

In order to install you need *cmake 3.2* and *tshark 1.9* (as part of wireshark). Unfortunately older tshark versions don't provide the MAC Address during probe-requests.
**pingpast** is using **PortAudio** for Audio I/O and relies on the **Boost Libraries** (>= 1.58.0). Both libraries are included precomplied for *Raspberry Pi* and *OS X*.

**Install**
```
mkdir build
cd build
cmake ../
make
```
**Use**

To use pings from the past, you must provide the wireless interface during launch. If **tshark** does not have permissions to run as root, you must launch **pingspast** with sudo.
```
sudo pingspast -i wlan0
```

