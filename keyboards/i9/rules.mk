# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   change yes to no to disable
#
PROGRAMMABLE_BUTTON_ENABLE = no
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
UNICODE_ENABLE = no	    # Enables unicode output
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = yes         # Commands for debug and configuration
NKRO_ENABLE = yes            # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
TAP_DANCE_ENABLE = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
ENCODER_ENABLE = yes
CUSTOM_MATRIX = lite
SRC += matrix.c

