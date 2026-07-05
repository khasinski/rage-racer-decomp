def apply(config, args):
    config["arch"] = "mipsel"
    config["baseimg"] = "assets/PAL/main.exe"
    config["myimg"] = "build/PAL/main.exe"
    config["mapfile"] = "build/PAL/main.map"
    config["source_directories"] = ["src", "include"]
    config["objdump_executable"] = "mipsel-none-elf-objdump"
    config["map_format"] = "gnu"
    config["build_command"] = ["make", "-j"]
