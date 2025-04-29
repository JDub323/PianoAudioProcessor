# Piano Audio Backend

This repository contains the **C++ backend** for a mobile application that interprets notes played on a physical piano in real time and provides feedback on performance accuracy.

## Project Overview

The goal of the project is to:
- Capture and analyze real-time audio from a piano.
- Detect the notes being played by extracting characteristics from the audio signal.
- Apply algorithms to evaluate how accurately the piece was performed and identify mistakes.

The **frontend** of the application is built using **Flutter** to enable cross-platform support (iOS, Android). This repository focuses exclusively on the **backend audio analysis engine**, written in C++.

## Current Status

- Implemented **Short-Time Fourier Transform (STFT)** analysis to extract key characteristics from recorded piano audio.
- Identified important features from the STFT output, primarily focusing on magnitude data.
- Working on evaluating a **greedy search algorithm** that attempts to match the sum of note magnitudes to determine which piano keys were played.

## Next Steps

- Assess the accuracy of the greedy search algorithm.
- If the greedy approach proves insufficient, enhance the system by:
  - Saving **phase information** during analysis.
  - Developing a method to **align phase data** between the recorded input and a reference database to more accurately identify played notes.

## Technologies Used

- **C++** — Core backend development
- **PortAudio**  — Audio capture
- **Flutter** — Frontend mobile application (separate repository)

## Future Plans

- Improve real-time performance and note recognition reliability.
- Integrate backend with the Flutter frontend via FFI (Foreign Function Interface).
- Implement advanced feedback algorithms to pinpoint timing, pitch, and dynamic inaccuracies.
