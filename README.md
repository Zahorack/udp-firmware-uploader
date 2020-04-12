# <div align="center">Remote firmware update management of embedded devices</div>

<br/>

<div align="center">Oliver Hollý</div>

<div align="center">Department of science and innovations, FEI STU in Bratislava</div>

<div align="center">Institute of automotive mechatronics</div>

<br/>
<br/>

**Abstract – Publication describes concept and practical implementation of remote firmware updates management for embedded and internet of things devices or systems. Firmware update feature is fundamental operation in development and operation process to reach required system nature. This paper has been created as a summary of knowledge acquired during participating on a real commercial product in my part time job**


## 1 Introduction
### 1.1 Motivation

The general motivation for developing remote firmware updates management infrastructure is to achieve the possibility to manage, upgrade or check firmware version of multiple devices connected to one centralized remote server. Reliable development and operation of embedded systems is based on testing and debugging firmware and its new versions and utilities.
<br/>
<br/>
We can split a software development process into two main stages. First stage begins with planning, analysis, design and finishes after development and implementation. Second stage of the software development process consist of testing and maintenance or service. First state is the best period for time and cost efficiency of software development process. In this first state we can afford to do lot of changes and adjustments based on client's needs.
<br/>
<br/>
It is a good practice to use debugging tools and boards with lots of interfaces and GUIs. But these tools are not available in state of maintenance and service, because of PCB size constrains, power consumption, computing capacity etc. And this is where our solution takes meaning. We need way to upgrade features of final product, deployed in operation miles away or just without needed additive wires and hardware.

<br/>.
<br/>.

## 2 Technical design and implementation

### [Read more](https://github.com/Zahorack/udp-firmware-uploader/blob/master/HOLLY_SVOC_2020.pdf)
<br/>.
<br/>.


# 3 Results

This publication has been created as a summary of knowledge
acquired during participating on a real commercial product in
my part time job. Company, I worked in, was focused on
automotive industry automation processes, especially quality
measurement. One of automation
solution used in quality measurement of combustion engines
heads.
<br/>

Embedded clients, further called probes, were capable
measure or read from analog and digital sensors and capture
image from micro camera module with 20 FPS rate. The
machine contains configuration of probes at specific positions
to achieve measurement of required spots.

<br/>
These probes had implemented remote firmware update
management system described in this publication. During
development process and operation, we have been able to
upload new firmware version to all probes (5 – 20 depends on
machine) in a short time and without mechanical manipulation.
<br/>

<img src="https://github.com/Zahorack/udp-firmware-uploader/img/probes.jpg" align="center" width="600" title="hover text">


