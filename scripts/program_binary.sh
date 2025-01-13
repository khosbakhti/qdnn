PROJECT_ROOT_DIR="/home/khan/Desktop/intermittent/imc_kernel_example_m33"
BIN_DIR="${PROJECT_ROOT_DIR}/build/imc_example"
NVM_RESET_DIR="${PROJECT_ROOT_DIR}/scripts/nvm_reset.elf"
OPENOCD_SCRIPT_DIR="${PROJECT_ROOT_DIR}/imc_freertos_app_m33.cfg"

cd $PROJECT_ROOT_DIR/build
cmake --build .
cd $PROJECT_ROOT_DIR
openocd -f ${OPENOCD_SCRIPT_DIR} -c "program ${NVM_RESET_DIR}" -c reset -c resume -c exit
sleep 5
openocd -f ${OPENOCD_SCRIPT_DIR} -c "program ${BIN_DIR}" -c reset -c resume -c exit
