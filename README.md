# cpp-MadLibs
A basic C++ program where you can choose from multiple pre-defined texts and fill in gaps madlibs style
## Installation
Clone this repository and compile the madlibs.cpp file.\
The put the .exe file in the same directory as /lib and /Saved\
You're Done!
## Add your own files
Currently you have to add a your text in a .txt file into the lib folder and add some code to the source.\
`const string YOUR_VARIABLE_NAME = ".\\lib\\madlibs_your_text_file.txt";`\
And then add `YOUR_VARIABLE_NAME` into `const vector<string> SCENARIO_LIST`\
I will add an automatic recognition of new files in the near future

## Disclaimer
This is my first program in c++ so the code might not be the cleanest.
So if you have any ideas on how to improve the code or features to add please let me know.
