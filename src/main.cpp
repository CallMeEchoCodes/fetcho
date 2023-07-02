#include <proc/sysinfo.h>
#include <pwd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define bold(str) (std::string("\e[1m") + str + "\e[22m")

#define red(str) (std::string("\e[91m") + str + "\e[0m")
#define yellow(str) (std::string("\e[93m") + str + "\e[0m")
#define green(str) (std::string("\e[92m") + str + "\e[0m")
#define cyan(str) (std::string("\e[96m") + str + "\e[0m")
#define blue(str) (std::string("\e[34m") + str + "\e[0m")
#define magenta(str) (std::string("\e[95m") + str + "\e[0m")

std::string getSymbol(std::string symbol, std::string altText) {
    std::string nerdFontEnvVar = getenv("FO_NERDFONTS") ? getenv("FO_NERDFONTS") : "";
    std::string term = getenv("TERM") ? getenv("TERM") : "";
    bool shouldUseNerdFonts;
    if (nerdFontEnvVar == "true" || nerdFontEnvVar == "1") {
        shouldUseNerdFonts = true;
    } else {
        shouldUseNerdFonts = false;
    }
    if (term == "linux") {
        shouldUseNerdFonts = false;
    }

    if (shouldUseNerdFonts)
        return symbol;
    else
        return altText;
}

std::string getDistro() {
    std::ifstream osReleaseFile;
    std::string distro = "Unknown";

    osReleaseFile.open("/etc/os-release");
    if (!osReleaseFile.is_open()) return distro;

    std::getline(osReleaseFile, distro);
    while (distro.find("PRETTY_NAME=") == std::string::npos) {
        std::getline(osReleaseFile, distro);
    }

    osReleaseFile.close();

    distro.erase(distro.find("PRETTY_NAME"), distro.find('=') + 1);
    distro.erase(0, 1);
    distro.erase(distro.size() - 1, distro.size());

    return distro;
}

std::string getUptime(struct sysinfo* sysInfo) {
    unsigned int uptime = sysInfo->uptime;
    if (uptime < 60) return std::to_string(uptime) + "s";
    if (uptime / 3600 == 0) return std::to_string(uptime % 3600 / 60) + "m";
    return std::to_string(uptime / 3600) + "h " + std::to_string(uptime % 3600 / 60) + "m";
}

std::string getShell(passwd* pw) {
    std::string shell = pw->pw_shell;
    shell.erase(0, shell.rfind('/') + 1);
    return shell;
}

std::string getDesktopEnvironment() {
    std::string xdgCurrentDesktop = std::getenv("XDG_CURRENT_DESKTOP") ? std::getenv("XDG_CURRENT_DESKTOP") : "";
    std::string desktopSession = std::getenv("DESKTOP_SESSION") ? std::getenv("DESKTOP_SESSION") : "";
    std::string term = getenv("TERM") ? getenv("TERM") : "";

    if (!xdgCurrentDesktop.empty()) {
        return xdgCurrentDesktop;
    } else if (!desktopSession.empty()) {
        return desktopSession;
    } else if (term == "linux") {
        return "None";
    } else {
        return "Unknown";
    }
}

std::string getMemory() {
    meminfo();
    double totalMemMebibytes = kb_main_total / 1024;

    // This should be 1024. But when i do that it outputs the incorrect value.
    // Maybe im just stupid but if someone can explain why this outputs the correct number of GIBIBytes then lmk
    double totalMemGibibytes = totalMemMebibytes / 1000;

    std::string totalSuffix;
    int totalDecimalPlaces;
    double finalTotalNumber;

    if (kb_main_total == 0) {
        totalSuffix = "B";
        finalTotalNumber = kb_main_total * 1024;
        totalDecimalPlaces = 0;
    } else if (totalMemMebibytes == 0) {
        totalSuffix = "K";
        finalTotalNumber = kb_main_total;
        totalDecimalPlaces = 0;
    } else if (totalMemGibibytes == 0) {
        totalSuffix = "M";
        finalTotalNumber = totalMemMebibytes;
        totalDecimalPlaces = 1;
    } else {
        totalSuffix = "G";
        finalTotalNumber = totalMemGibibytes;
        totalDecimalPlaces = 1;
    }

    double usedMemMebibytes = kb_main_used / 1024;
    double usedMemGibibytes = usedMemMebibytes / 1000;

    std::string usedSuffix;
    int usedDecimalPlaces;
    double finalUsedNumber;

    if (kb_main_used == 0) {
        usedSuffix = "B";
        finalUsedNumber = kb_main_used * 1024;
        usedDecimalPlaces = 0;
    } else if (usedMemMebibytes == 0) {
        usedSuffix = "K";
        finalUsedNumber = kb_main_used;
        usedDecimalPlaces = 0;
    } else if (usedMemGibibytes == 0) {
        usedSuffix = "M";
        finalUsedNumber = usedMemMebibytes;
        usedDecimalPlaces = 1;
    } else {
        usedSuffix = "G";
        finalUsedNumber = usedMemGibibytes;
        usedDecimalPlaces = 1;
    }

    std::ostringstream stringStream;
    stringStream << std::fixed << std::setprecision(usedDecimalPlaces) << finalUsedNumber << usedSuffix << " / " << std::setprecision(totalDecimalPlaces) << finalTotalNumber << totalSuffix;
    return stringStream.str();
}

std::string makeLine(std::string text) {
    std::string line = "";
    char* lineTextEnvVar = getenv("FO_LINETEXT");
    std::string lineText = lineTextEnvVar ? std::string(lineTextEnvVar) : "─";

    for (unsigned int i = 0; i < text.length(); i++) {
        line.append(lineText);
    }
    return line;
}

std::vector<std::string> getOptions() {
    char* modulesEnvVar = getenv("FO_MODULES");

    if (!modulesEnvVar) return {"os", "kernel", "uptime", "shell", "ram", "de"};

    std::stringstream stream;
    stream << modulesEnvVar;

    std::vector<std::string> optionsVector;
    std::string currentLine;

    while (std::getline(stream, currentLine, ' ')) {
        optionsVector.push_back(currentLine);
    }

    return optionsVector;
}

int main() {
    struct utsname un;
    uname(&un);

    passwd* pw = getpwuid(geteuid());

    std::vector<std::string> optionsVector = getOptions();

    std::string hostname = un.nodename;
    std::string username = pw->pw_name;
    std::string userInfo = bold(magenta(username)) + bold(green("@")) + bold(magenta(hostname));
    std::cout << userInfo << std::endl;
    std::cout << bold(makeLine(username + "@" + hostname)) << std::endl;

    for (long unsigned int i = 0; i < optionsVector.size(); i++) {
        std::string currentString = optionsVector[i];

        if (currentString == "os") {
            std::cout << bold(red(getSymbol("  ", "os     "))) << getDistro() << std::endl;
        } else if (currentString == "kernel") {
            std::cout << bold(yellow(getSymbol("  ", "kernel "))) << std::string(un.sysname) + " " + std::string(un.release) << std::endl;
        } else if (currentString == "uptime") {
            struct sysinfo* sysInfo;
            sysinfo(sysInfo);
            std::cout << bold(green(getSymbol("  ", "uptime "))) << getUptime(sysInfo) << std::endl;
        } else if (currentString == "shell") {
            std::cout << bold(cyan(getSymbol("  ", "shell  "))) << getShell(pw) << std::endl;
        } else if (currentString == "ram") {
            std::cout << bold(blue(getSymbol("  ", "ram    "))) << getMemory() << std::endl;
        } else if (currentString == "de") {
            std::cout << bold(magenta(getSymbol("  ", "de     "))) << getDesktopEnvironment() << std::endl;
        } else {
            std::cerr << red("\"") << red(currentString) << red("\"") << red(" is not a valid module!") << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
