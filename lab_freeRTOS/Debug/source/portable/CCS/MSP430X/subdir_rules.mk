################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
source/portable/CCS/MSP430X/%.obj: ../source/portable/CCS/MSP430X/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs930/ccs/ccs_base/msp430/include" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS/source/portable/CCS/MSP430X" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS/source/include" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS" --include_path="/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="source/portable/CCS/MSP430X/$(basename $(<F)).d_raw" --obj_directory="source/portable/CCS/MSP430X" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source/portable/CCS/MSP430X/%.obj: ../source/portable/CCS/MSP430X/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs930/ccs/ccs_base/msp430/include" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS/source/portable/CCS/MSP430X" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS/source/include" --include_path="/Users/ptmascarenhas/workspace_v9/lab_freeRTOS" --include_path="/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="source/portable/CCS/MSP430X/$(basename $(<F)).d_raw" --obj_directory="source/portable/CCS/MSP430X" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


