################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --include_path="H:/Workspace/ELEX7890/F28027_I2C_LCD" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="H:/Workspace/ELEX7890/RemoteSystemsTempFiles/f2802x_common/include" --include_path="H:/Workspace/ELEX7890/RemoteSystemsTempFiles/f2802x_common/source" --include_path="H:/Workspace/ELEX7890/RemoteSystemsTempFiles/f2802x_headers/include" --include_path="H:/Workspace/ELEX7890/RemoteSystemsTempFiles/f2802x_headers/source" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


