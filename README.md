# imcFreeRTOS Demo

## Build

### 구성
- 본 프로젝트에는 FreeRTOS V10.3.1 (STM CubeIDE 에서 생성된 버전) 이 포함되어 있음
- 이 중 3개 파일이 체크포인트 기능을 지원하기 위해 변경되었으며, 변경된 파일은 라이브러리로 제공됨
    - `Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/port.c`
    - `Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c`
    - `Middlewares/Third_Party/FreeRTOS/Source/tasks.c`
- 따라서 위 세 파일을 제외하고 컴파일을 수행한 후 제공된 `lib/libimc_freeRTOS.a` 를 링크하여 최종 바이너리 빌드 가능
- 리눅스 환경에서 실행가능한 `cmake` 파일이 제공되었으며 위 방식으로 빌드를 수행하면 기타 시스템에서도 사용 가능

### Linux 환경에서 빌드하기
#### Arm GNU Toolchain 설치하기

- `apt` 등의 패키지 관리자로 설치할 경우 헤더 등 요소가 설치되지 않으므로 전체 toolchain 설치 필요
- Arm GNU Toolchain Downloads 페이지(<https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads>)에서 호스트 환경에 맞는 toolchain 다운로드
	- Target: **AArch32 bare-metal target (arm-none-eabi)**
- 리눅스 환경의 경우, 원하는 위치에 압축을 풀고 `bin` 디렉토리를 `PATH` 에 추가
	- e.g. `/opt/gcc-arm-none-eabi` 에 압축을 해제하는 경우
		- `sudo mkdir /opt/gcc-arm-none-eabi`
		- `sudo tar xf gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi`
	- `PATH` 에 추가: `~/.zshrc` 등을 연 후 `export PATH=$PATH:/opt/gcc-arm-none-eabi/bin` 추가
	- 로그아웃 후 로그인

#### `CMakeLists.txt` 파일 수정하기

`CMakeLists.txt` 파일 상단에 **Host specific settings** 에 설정되어 있는 변수를 설정해야 함

- `CMAKE_C_COMPILER`: 사용할 컴파일러 설정. `clang` 및 `arm-none-eabi-gcc` 로 테스트됨
- `CMAKE_SYSROOT`: 툴체인을 검색할 root directory. 본 예시에서는 `/opt/gcc-arm-none-eabi/arm-none-eabi`
- `ARM_CORTEXM_MULTI_DIR`: `libc.a`, `libm.a` 등의 standard library 를 찾을 디렉토리. 본 예시에서는 `/opt/gcc-arm-none-eabi/arm-none-eabi/lib/thumb/v7e-m+fp/hard`
- `ARM_CORTEXM_BUILTINS_DIR`: 컴파일러가 제공하는 builtin 함수들의 구현으로, `-lgcc` 에서 사용됨. 본 예시에서는 `/opt/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/11.2.1/thumb/v7e-m+fp/hard`

컴파일러 기반 체크포인트 삽입 기능을 활용할 경우 추가적으로 LLVM 의 `opt`, `llc` 위치를 지정할 수 있음
- `OPT_BIN`: 사용할 `opt` 바이너리의 위치
- `LLC_BIN`: 사용할 `llc` 바이너리의 위치
- `LLVM_OPT_PLUGIN`: 기본값 사용

#### `cmake` 를 사용하여 빌드하기

- 프로젝트 루트 디렉토리에서 빌드 명령 실행

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

- 빌드된 바이너리 확인하기
```bash
$ file imc_example
imc_example: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), statically linked, with debug_info, not stripped
```


## 체크포인트 활용하기

- `Core/Src/main.c` 내의 task 생성 API 변경
    - 기존 `osThreadNew()` 대신 `imcOsThreadNew()` 사용, 파라미터는 동일
    - `imcOsThreadNew()` 로 생성된 태스크는 태스크 생성 시 저장된 체크포인트가 있는지 확인하고 있을 경우 복구 (stack 및 register)
    - 체크포인트가 필요하지 않은 태스크는 기존 `osThreadNew()` 활용 가능

### 수동 체크포인트 삽입
- 소스 코드의 원하는 위치에 직접 체크포인트를 삽입
- 응용 코드 내에서 체크포인트를 원하는 지점에 체크포인트 호출 매크로 (`imcREQUEST_CHECKPOINT()`) 삽입
    - 예시:
    ```c
    void taskImcTest(void *argument) {
        int i = 0;
        while(1) {
            osDelay(1000);
            printf("i=%d\r\n", i++);
            imcREQUEST_CHECKPOINT();
        }
    }
    ```

### 컴파일러 기반 체크포인트 삽입
- 본 기능은 LLVM 17.0.6 버전에 사용할 수 있는 plugin 형태로 제공
- 매 loop iteration 마다 자동으로 체크포인트 삽입
- 체크포인트 수행 빈도를 조절할 수 있는 두 가지 옵션이 존재하며, `Core/Inc/ImC/imc_kernel.h` 내에서 설정 가능 (`cmake` 스크립트를 사용 시에는 CMakeLists.txt 내에서 설정해야 함)
    - `imcUSE_CHECKPOINT_PASS_COUNTER`: 옵션이 활성화되어 있을 경우 모든 체크포인트를 수행하는 대신 `imcCHECKPOINT_PASS_COUNT` 에 정의된 횟수마다 한번씩 체크포인트 실행.
        - 예시: `imcCHECKPOINT_PASS_COUNT`=100 일 경우 한 번 체크포인트 실행 이후 실행되는 100회의 체크포인트를 스킵
    - `imcUSE_CHECKPOINT_VOLTAGE_CHECK`: 체크포인트 수행 시 EPS ch0 에 연결된 커패시터 전압을 체크한 후 이 값이 `imcCHECKPOINT_VOLTAGE` 보다 낮을 경우에만 체크포인트 수행
- Plugin 은 LLVM `opt` 의 `-load-pass-plugin` 옵션으로 사용할 수 있으며 `-imc-insert-checkpoints` 옵션을 사용하여 기능 활성화
    - 구체적인 수행 방식은 CMakeLists.txt 내 코드 참조

## 기타/참고사항
- 현재 `imcOsThreadNew()` 로 동시에 생성할 수 있는 태스크는 5개로 제한되어 있으며, 각 태스크의 최대 스택 크기는 4096 bytes 로 제한되어 있음
- 저장된 체크포인트를 초기화하기 위해서 NVM 을 초기화할 경우, `scripts/program_binary.sh` 사용 가능