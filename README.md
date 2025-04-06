# FIELD-4D: A Phenotyping Framework for Precision Agricultural Research

---

<div >
<img src="https://avatars.githubusercontent.com/u/71197432?s=400&u=837258e8d4c3b42fe4d44789e3602a3d83b64b61&v=4" alt="F4D Logo" width="230" height="240">
 <img src="https://docs.contiki-ng.org/en/master/_static/contiki_logo.png" alt="Contiki-NG Logo" width="256"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/d0/Yellow_ribbon.svg/200px-Yellow_ribbon.svg.png" alt="Yellow Ribbon Logo" width="80">
</div>
---

# FIELD-4D IoT Repository

This repository is a core component of the FIELD-4D ecosystem, providing the IoT infrastructure for real-time outdoor phenotyping and greenhouse sensor monitoring. It integrates embedded firmware, sensor network implementations, and communication protocols for efficient field data acquisition and edge processing.

## Components Overview

### Sensors Platform (SP) v1

- **Purpose:**
  A low-power child node for environmental data acquisition. SP v1 collects sensor readings—such as temperature, humidity, and light intensity—and transmits data over UDP using RPL-TSCH.

- **Hardware:**
  TI CC2650 SensorTag (CC2650STK).

- **Software Environment:**
  Utilizes the SimpleLink software environment with modular TI Drivers, NoRTOS support, and a Core SDK provided as a git submodule.

- **Build Instructions:**

  ```sh
  make distclean
  make TARGET=cc26x0-cc13x0 BOARD=sensortag/cc2650
  ```

---

### Base Edge Router (BER)

- **Purpose:**
  Serves as the RPL root node, TSCH coordinator, and UDP server. BER ensures efficient power management, minimal data loss, and continuous spatiotemporal data flow using UDP-to-JSON encapsulation.

- **Hardware:**
  TI CC2650 LaunchPad.

- **Software Environment:**
  Similar SimpleLink software environment with TI Drivers and configurable memory/code optimization options via `project-conf.h`.

- **Build Instructions:**

  ```sh
  make distclean
  make TARGET=simplelink BOARD=launchpad/cc2650
  ```

---

### 6to4 Sensors Project

- **Purpose:**
  Facilitates efficient data collection, processing, and communication from wireless IoT sensors deployed in greenhouse environments.

- **Key Features:**

  - **Real-time Data Acquisition:** Measures plant physiological and environmental parameters including temperature, humidity, light intensity, and barometric pressure.
  - **Wireless Sensor Network:** Implements a 6LoWPAN mesh network to support low-energy sensor operations (up to three months on two AA batteries).
  - **Data Processing and Cloud Integration:** Processes data locally on a Raspberry Pi receiver, formats outputs to CSV, and optionally uploads data to MongoDB and Google Cloud Platform.
  - **Visualization:** Includes a Streamlit dashboard for real-time and historical sensor data visualization.

- **Example Directory Structure:**

  ```plaintext
  6to4-sensors/
  ├── 6to4client
  ├── 6to4server
  ├── Dashboard
  ├── MongoUpload
  ├── initializer
  ├── services
  ├── requirements.txt
  ```

## Getting Started

1. **Clone the Repository:**

   ```sh
   git clone https://github.com/field-4d/field-4d-iot.git
   cd field-4d-iot
   ```

2. **Review the Documentation:**
   Detailed build instructions, configuration settings, and deployment guidelines are available in the `docs` directory.

## Contributing

Contributions are welcome. Please review the `CONTRIBUTING.md` file for guidelines on reporting issues and submitting pull requests.

## License

This repository is part of the FIELD-4D ecosystem. Usage and distribution are governed by the license provided in the root directory. Specific components such as SP v1, BER, and the 6to4 Sensors project are licensed under the MIT License.

## Contact

For further information or collaboration inquiries, please contact:

- **Email:** [greenroom.lab@mail.huji.ac.il](mailto:greenroom.lab@mail.huji.ac.il)

---

FIELD-4D IoT: Bridging cutting-edge technology with precision agricultural innovation.

---
