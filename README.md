# F4D (Field 4D)

Field 4D Firmware and Driver

## Table of Contents

- [F4D (Field 4D)](#f4d-field-4d)
  - [Table of Contents](#table-of-contents)
  - [Reference Guides](#reference-guides)
  - [Usage](#usage)
  - [Folder Structure](#folder-structure)
    - [Key Directories and Files](#key-directories-and-files)
  - [VS Code Configuration](#vs-code-configuration)

## Reference Guides


- [New Sensors Platform schematics](pdf/PLS001-SCH-001-R01.pdf)
  - [Sensors Map](pdf/AI4DPCB-SPEC-001-R01.pdf)

---

- [BMP Sensor Guide](pdf/bosch-bmp390l-datasheet.pdf)
- [OPT Sensor Guide](pdf/opt3001.pdf)
- [HDC2010 Sensor Guide](pdf/hdc2010.pdf)

## Usage

How to use your project.

## Folder Structure

The project is divided into two main components:

- `sp` (Sensors Platform): This is the client part of the project. It contains sensor-related code and configurations.
- `gw` (Base Edge Router): This directory holds the code for the Base Edge Router, which acts as an intermediary between the sensors and the network.

### Key Directories and Files

- `sp/`
  - `sp_v_1_9_0.c`: Main source file for the Sensors Platform.
  - `Makefile`: Used for compiling the Sensors Platform.
  - `project-conf.h`: Configuration header file for the Sensors Platform.
- `gw/`: Contains files and configurations for the Base Edge Router.

## VS Code Configuration

Below is the Visual Studio Code configuration for this project:

```json
{
    "configurations": [
        {
            "name": "Mac",
            // More configuration details...
        },
        {
            "name": "Contiki-NG",
            // More configuration details...
        }
    ],
    "version": 4
}
