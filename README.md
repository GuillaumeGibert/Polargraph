# Polargraph

## About The Project

[![Project Screen Shot][project-screenshot]]()

I propose this project to engineering students who want to discover robotics. During this 2-days project, students work on image processing, mecahnics, electronics and motor control. They assemble the system and test it thanks to the provided Arduino code. Then, they learn how to extract image contours using [Python](https://www.python.org/) and the [OpenCV library](https://opencv.org/). Finally, they convert cartesian coordinates of the contours to numbers of steps to control the two stepper motors. The pen can be lift up/down thansk to a servo motor activated between contours.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![Arduino][arduino-shield]][arduino-url]
* [![Python][python-shield]][python-url]
* [![OpenCV][opencv-shield]][opencv-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To set up the project locally, you need to install (if not already the case) some dependencies.
To get a local copy up and running follow these steps.

### Prerequisites

* Arduino

Launch Arduino IDE and install the library Adafruit Motor Shield v2

* Python

Install the pySerial library
  ```sh
  pip install pyserial
  ```
  
Install the OpenCV library
  ```sh
  pip install opencv-python
  ```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/GuillaumeGibert/Polargraph.git
   ```
2. Open Arduino IDE
3. Open the code `testSetup.ino`
4. Compile and upload it to your Arduino board

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

### Check the position of the stepper motors

1. Open Arduino IDE
2. Open the code `testStepperMotors.ino`
3. Compile and upload it to your Arduino board
First, motor A spins forward during a full revolution than backward. then, motor B spins backward during a full revolution than forward.

### Test the Arduino code
1. Open Arduino IDE
2. Open the code `testSetup.ino`
3. Compile and upload it to your Arduino board
4. Open the Serial monitor
5. Set the baudrate to 115200 bauds
6. Write 
```sh
Start
```
```sh
Data:200;0
```
The pen moves towards the right of 200 mm.
```sh
Stop
```
The pen moves back to its initial position.

### Draw the image contour

5. Launch the python code:
   ```sh
   python imageProcessing.py -i <imageFilename> -f <fps> -p <comPort> -b <baudrate> -s <scaling> -c <sampling>
   ```
By using arguments, you can modify the image filename, the fps (frames per second), the COM port, the Baudrate, the scaling and sampling factors.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- LICENSE -->
## License

Distributed under the GPL License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Guillaume Gibert

Project Link: [https://github.com/GuillaumeGibert/Polargraph](https://github.com/GuillaumeGibert/Polargraph)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[arduino-shield]: https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white
[arduino-url]: https://www.arduino.cc/
[python-shield]: https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white
[python-url]: https://www.python.org/
[opencv-shield]: https://img.shields.io/badge/OpenCV-27338e?style=for-the-badge&logo=OpenCV&logoColor=white
[opencv-url]: https://opencv.org/

[project-screenshot]: images/screenshot.png

[contributors-shield]: https://img.shields.io/github/contributors/GuillaumeGibert/Polargraph.svg?style=for-the-badge
[contributors-url]: https://github.com/GuillaumeGibert/Polargraph/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/GuillaumeGibert/Polargraph.svg?style=for-the-badge
[forks-url]: https://github.com/GuillaumeGibert/Polargraph/network/members
[stars-shield]: https://img.shields.io/github/stars/GuillaumeGibert/Polargraph.svg?style=for-the-badge
[stars-url]: https://github.com/GuillaumeGibert/Polargraph/stargazers
[issues-shield]: https://img.shields.io/github/issues/GuillaumeGibert/Polargraph.svg?style=for-the-badge
[issues-url]: https://github.com/GuillaumeGibert/Polargraph/issues
[license-shield]: https://img.shields.io/github/license/GuillaumeGibert/Polargraph.svg?style=for-the-badge
[license-url]: https://github.com/GuillaumeGibert/Polargraph/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/guillaume-gibert-06502ba4
