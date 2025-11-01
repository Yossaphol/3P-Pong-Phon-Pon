idf_component_register(SRCS "cmd_nvs.c"
                       INCLUDE_DIRS "."
                       REQUIRES nvs_flash console cmd_router)
