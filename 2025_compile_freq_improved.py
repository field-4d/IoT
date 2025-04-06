import os
import re
import shutil
import time
import logging

# Define the main path dynamically
main_path = os.path.abspath(r'//mnt/c/Users/Moshelion/OneDrive/Desktop/2025_f4d')

# Setup logging
log_dir = os.path.join(main_path, '2025_python_logs')
os.makedirs(log_dir, exist_ok=True)
log_file = os.path.join(log_dir, '2025_script.log')

# Configure logging
logging.basicConfig(filename=log_file, level=logging.INFO, 
                    format='%(asctime)s:%(levelname)s:%(message)s')
logger = logging.getLogger(__name__)


def create_dir(destination_dir):
    """
    Creates a directory if it does not exist.
    """
    destination_dir = os.path.abspath(destination_dir)
    if not os.path.exists(destination_dir):
        logger.info(f"Creating directory: {destination_dir}")
        os.makedirs(destination_dir, exist_ok=True)
    else:
        logger.info(f"Directory already exists: {destination_dir}")


def update_panid(source_file, new_panid):
    """
    Update the PANID value in the specified source file.
    """
    source_file = os.path.abspath(source_file)

    if not os.path.exists(source_file):
        logger.error(f"Source file not found: {source_file}")
        return
    
    logger.info(f"Reading the file: {source_file}")
    
    # Read the content of the file
    with open(source_file, 'r') as file:
        content = file.read()

    # Use regex to find and replace the PANID value
    pattern = re.compile(r'#define IEEE802154_CONF_PANID 0x[0-9A-Fa-f]+')
    new_content = pattern.sub(f'#define IEEE802154_CONF_PANID {new_panid}', content)

    # Write the modified content back to the file
    with open(source_file, 'w') as file:
        file.write(new_content)

    logger.info("File updated successfully.")


def compile_launchpad(new_panid, gw_dir):
    """
    Compile the launchpad binary.
    """
    gw_dir = os.path.abspath(gw_dir)
    logger.info(f"Changing directory to: {gw_dir}")

    if not os.path.exists(gw_dir):
        logger.error(f"GW directory does not exist: {gw_dir}")
        return
    
    os.chdir(gw_dir)

    logger.info("Ensuring previous project is cleaned")
    os.system("make disclean")

    logger.info("Make command executing")
    os.system("make")

    os.chdir(main_path)

    destination_dir = os.path.join(main_path, "2025_firmware", new_panid)
    create_dir(destination_dir)

    compiled_file_path = os.path.join(gw_dir, "build/simplelink/launchpad/cc2650/ber.bin")
    
    if os.path.exists(compiled_file_path):
        shutil.copy(compiled_file_path, os.path.join(destination_dir, f"GW_{new_panid}.bin"))
        logger.info(f"Copied compiled file to {destination_dir}")
    else:
        logger.error(f"Compiled file not found: {compiled_file_path}")


def compile_SP(new_panid, sp_dir):
    """
    Compile the SP binary.
    """
    sp_dir = os.path.abspath(sp_dir)
    logger.info(f"Changing directory to: {sp_dir}")

    if not os.path.exists(sp_dir):
        logger.error(f"SP directory does not exist: {sp_dir}")
        return

    os.chdir(sp_dir)

    logger.info("Ensuring previous project is cleaned")
    os.system("make disclean")

    logger.info("Make command executing")
    os.system("make")

    os.chdir(main_path)

    destination_dir = os.path.join(main_path, "2025_firmware", new_panid)
    create_dir(destination_dir)

    compiled_file_path = os.path.join(sp_dir, "build/cc26x0-cc13x0/sensortag/cc2650/sp_v1.bin")

    if os.path.exists(compiled_file_path):
        shutil.copy(compiled_file_path, os.path.join(destination_dir, f"SP_{new_panid}.bin"))
        logger.info(f"Copied compiled file to {destination_dir}")
    else:
        logger.error(f"Compiled file not found: {compiled_file_path}")


def main(new_panid):
    try:
        start_time = time.time()

        # Define paths
        gw_dir = os.path.join(main_path, "system-arch/measurement/ber")
        sp_dir = os.path.join(main_path, "system-arch/measurement/sensors-platforms/v1")
        source_file_gw = os.path.join(gw_dir, "project-conf.h")
        source_file_sp = os.path.join(sp_dir, "project-conf.h")

        logger.info(f"Destination directory: {os.path.join(main_path, '2025_firmware', new_panid)}")

        # Update PANID and compile firmware
        update_panid(source_file_gw, new_panid)
        compile_launchpad(new_panid, gw_dir)

        update_panid(source_file_sp, new_panid)
        compile_SP(new_panid, sp_dir)

        end_time = time.time()
        duration = end_time - start_time

        logger.info(f"Compilation completed in {duration:.2f} seconds")
        print(f"Compilation completed in {duration:.2f} seconds")

    except Exception as e:
        logger.error(f"Error occurred: {e}")
        print(f"Error: {e}")


if __name__ == '__main__':
    for i in range(56, 60):
        new_panid = f'0x{i:04}'
        print(f"Processing PANID: {new_panid}")
        main(new_panid)
