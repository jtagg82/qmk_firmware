# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   change yes to no to disable
#
EXTRAKEY_ENABLE = yes		# Audio control and System control
NKRO_ENABLE = yes           # Enable N-Key Rollover
OLED_ENABLE = yes
OLED_DRIVER = ssd1306
ENCODER_ENABLE = yes
CUSTOM_MATRIX = lite
SRC += matrix.c
