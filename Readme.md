- - -
# TADMF:Trained API Dependency Model-based Fuzzer
TADMF is Windows Kernel API Fuzzer
#### made by team JJ
- - -

## Architecture

![아키텍쳐](https://user-images.githubusercontent.com/49066484/157186779-3f403eb2-7630-4379-af06-649c8e6f20b9.png)

---

## How to Build/Run?

### Requirements
- Python 3.7.4
- Vmware 15.5.1
- Vmware VIX 1.17
- Windows 10 iso file

### Step 1. set monitor.py
>1. Install Windows 10 on VMware.
>
>2. Create a Shared Folder on VMware.
>
>3. Get IP Address on VMware.
>
>4. Set vmx path, shared folder path, ip address in Monitor/Monitor.py

### Step 2. parse header
>1. Use parse.py for making header.txt, s_information.csv 
>
>2. header.txt is about WINGDIAPI data
>
>3. s_information.csv is about structure data

### Step 3. hook application
>1. Use hook.py for hooking application
>
><pre><code>py hook.py [output_path] [target_program] </code></pre>
>
>2. output file name must be "~~/Log0~1.txt"


### Step 4. make ADT(Api Dependency Tree)
>1. Use detect.py for making ADT
>
>2. you can skip this step. just use generate.py

### Step 5. generate API call sequence.
>1. Use generate.py for making CPP file<br>
>
><pre><code>py generate.py [# of log set] [use existing model(y/n)]</code></pre>
>
>2. generate code and compile it
>
>3. the result is on output/Fuzzer.exe


### Step 6. set run.py
>1. Set shared folder path
>
>2. install pyinstaller
><pre><code>pip install pyinstaller</code></pre>
>
>3. make run.exe
><pre><code>pyinstaller --onefile run.py</code></pre>

### Fuzzing
>1. Copy 'run.exe' and 'Fuzzer.exe' to VMware (C:\Users\HooN\Desktop\untitled3\*)
>
>2. Turn off Guest not vmware
>
>3. Start in Host
><pre><code>py Monitor.py</code></pre>

- - -

## How to Train?

### 1. using hook.py (Logging)
### 2. using detect.py (arrange in proper length)
### 3. using generate.py (make Cpp code)

---

## Demo

![ezgif com-gif-maker](https://user-images.githubusercontent.com/49066484/157182906-436295b2-0552-4fa8-bcb4-4d91cd3c5499.gif)

