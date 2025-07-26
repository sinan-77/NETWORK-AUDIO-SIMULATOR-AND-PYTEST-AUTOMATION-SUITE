NETWORKED AUDIO DEVICE SIMULATOR & PYTEST AUTOMATION SUITE
Summary
This project is an end-to-end demonstration of software quality assurance for a networked system, showcasing versatile testing methodologies.

The core application is a multi-client, asynchronous server written in C++ using the Asio library, which simulates a network-controllable audio device. This implementation is validated by a Python test automation suite built with the pytest framework, ideal for rapid, system-level functional testing. However, the architecture is designed to also be tested at the unit and integration level directly within C++ using frameworks like Google Test.

The project showcases proficiency in both C++ and Python, asynchronous programming, and a comprehensive understanding of testing principles—from high-level Python automation to low-level C++ unit testing.

Features
C++ Device Simulator (device_simulator/)

Asynchronous Server: Built with Asio to handle multiple simultaneous client connections efficiently without blocking.

State Management: Maintains an internal state for device properties (e.g., volume).

Command-Based API: Responds to simple text-based commands over a TCP socket (e.g., GET_VOLUME, SET_VOLUME).

Robust Error Handling: Gracefully handles invalid commands and arguments.

Python Test Automation Suite (automation_suite/)

Structured Tests: Uses the pytest framework to define clear, maintainable test cases.

Independent Tests: Employs pytest fixtures to ensure each test runs with a fresh, clean connection to the server.

Comprehensive Coverage: Includes positive tests (happy path), negative tests (error conditions), and tests for unknown commands.

Automated Validation: Uses assert statements to automatically verify server responses, providing clear pass/fail results.

Tech Stack
Language: C++14, Python 3

Networking/Async: Asio (Header-only library)

Testing Framework: pytest

Compiler: g++ (MinGW-w64 on Windows)

Getting Started
Follow these instructions to get the project running on your local machine.

Prerequisites
C++ Compiler: A modern C++ compiler that supports C++14 (e.g., g++, Clang, MSVC). For Windows, MinGW-w64 is recommended.

Python: Python 3.6 or newer.

Git: (Optional, for version control).

Setup & Execution
Clone the Repository (if on GitHub)

git clone https://github.com/YourUsername/qsc-sqa-project.git
cd qsc-sqa-project

Download Asio Library

Download the latest header-only version of Asio from the Asio website.

Unzip the contents into the device_simulator/ directory. Your folder structure should look like device_simulator/asio-1.34.2/.

Compile the C++ Device Simulator

Open a terminal and navigate to the device_simulator directory.

Run the following command to compile the server. Replace asio-1.34.2 with your specific version if different.

On Windows (with MinGW):

g++ main.cpp -o simulator -std=c++14 -I asio-1.34.2/include -lws2_32 -lwsock32

On macOS/Linux:

g++ main.cpp -o simulator -std=c++14 -I asio-1.34.2/include -lpthread

Run the C++ Server

In the same terminal, run the compiled application. It will wait for connections.

./simulator.exe  # On Windows
./simulator      # On macOS/Linux

You should see the message: Asynchronous device simulator running on port 12345...

Set Up and Run the Python Test Suite

Open a new, separate terminal window.

Navigate to the automation_suite directory.

Install the necessary Python packages:

pip install pytest

Run the tests:

pytest -v

View the Results

The Python terminal will show the results of the test run (e.g., 4 passed).

The C++ server terminal will show the commands it received from the test suite.

How It Works
The project operates on a simple client-server model. The C++ application runs as a persistent TCP server, listening for connections on port 12345. When the pytest script is executed, it acts as a client. For each test function, it establishes a new TCP connection, sends a specific command, waits for a response, and then asserts that the response matches the expected outcome before closing the connection. The asynchronous nature of the C++ server allows it to handle these rapid connections from the test suite without issue.