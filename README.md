Open Fit
==================

This algorithm is a simple C++ program aimed to help bike fit professionals with the measurements of the cyclist fit into the bicycle. It is capable of estimate the angles within the joins of the athlet during the motion of pedalling. It uses the famous and widely used OpenCV library. With the information provided by the software, the professional can analise accuratly and make a proper fit of the cyclist. This project was developed in the context of the Vision Systems Class with my classmate Felipe Tabata Fukushima.

---

## How to Compile and Run the Software

The program was developed on C++ language and It's excecution was validated using the GCC compiler. It is recommended to compile on GCC because errors may raise if using another compiler. 
GCC is multiplataform (i.e. runs on Windows, Linux or MAC) and so the Open Fit works on these plataforms as long as compiled on GCC. 

To run the tool and perform the data conversion, the path to the input file (video format) should be informed while excecuting the program.

The initial position of the markers on the cyclist joint must be informed by a simple click on them (hand, elbow, shoulder, hip, knee, ankle).

### The Input Data Format

To the algorithm be able to perform it's operations, a video file must be recorded with the cyclist wearing markers along the joints were the angles will be measured. The marker should be a round target like big enought to be detected by the software (the size depends on the video quality and vice versa, for the purpose of testing, a 50 mm black marker was used). Based on test as well, it's recomended to record the video in at least 60 FPS or record it in slow motion to enable the markers tracking. 

### The Output Information

The information provided by the software will be the minimium and maximum angles achieved during the following joints movement:

- Elbow 
- Shoulder
- Hip
- Knee

---

### Suggestions for Future Development

A good addiction to this tool would be to develop a GUI and enable real time video processing, as well as test for LED indicators, which are less chunky.

### Images

#### Algorithm Work Principle
<img src="https://github.com/rodrigojost/openFit/main/assets/software.png">

#### Image Results
<img src="https://github.com/rodrigojost/openFit/main/assets/imageResults.png">

#### Calculated Results
<img src="https://github.com/rodrigojost/openFit/main/assets/mathResults.png">
