#include <string>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <proc/sysinfo.h>
#include <sstream>
#include <iomanip>
#include <map>

#define bold(str) (std::string("\e[1m") + str + "\e[22m")

#define red(str) (std::string("\e[91m") + str + "\e[0m")
#define yellow(str) (std::string("\e[93m") + str + "\e[0m")
#define green(str) (std::string("\e[92m") + str + "\e[0m")
#define cyan(str) (std::string("\e[96m") + str + "\e[0m")
#define magenta(str) (std::string("\e[95m") + str + "\e[0m")

std::string getSymbol(std::string symbol, std::string altText) {
    std::string nerdFontEnvVar = getenv("QF_NERDFONTS") ? getenv("QF_NERDFONTS") : "";
    std::string term = getenv("TERM") ? getenv("TERM") : "";
    bool shouldUseNerdFonts;
    if (nerdFontEnvVar == "true" || nerdFontEnvVar == "1") { shouldUseNerdFonts = true; } else { shouldUseNerdFonts = false; }
    if (term == "linux") { shouldUseNerdFonts = false; }

    if (shouldUseNerdFonts) return symbol; else return altText;
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

std::string getUptime(struct sysinfo sysInfo) {
    unsigned int uptime = sysInfo.uptime;
    if (uptime < 60) return std::to_string(uptime) + "s";
    if (uptime / 3600 == 0) return std::to_string(uptime % 3600 / 60) + "m";
    return std::to_string(uptime / 3600) + "h " + std::to_string(uptime % 3600 / 60) + "m";
}

std::string getShell(passwd* pw) {
    std::string shell = pw->pw_shell;
    shell.erase(0, shell.rfind('/') + 1);
    return shell;
}

std::string getMemory() {
    meminfo();
    unsigned long totalMemKibibytes = kb_main_total;
    double totalMemBytes = totalMemKibibytes * 1024;
    double totalMemMebibytes = totalMemKibibytes / 1024;

    // This should be 1024. But when i do that it outputs the incorrect value.
    // Maybe im just stupid but if someone can explain why this outputs the correct number of GIBIBytes then lmk
    double totalMemGibibytes = totalMemMebibytes / 1000;

    std::string totalSuffix;
    int totalDecimalPlaces;
    double finalTotalNumber;

    if (totalMemKibibytes == 0) { totalSuffix = "B"; finalTotalNumber = totalMemBytes; totalDecimalPlaces = 0; }
    else if (totalMemMebibytes == 0) { totalSuffix = "K"; finalTotalNumber = totalMemKibibytes; totalDecimalPlaces = 0; }
    else if (totalMemGibibytes == 0) { totalSuffix = "M"; finalTotalNumber = totalMemMebibytes; totalDecimalPlaces = 1; }
    else { totalSuffix = "G"; finalTotalNumber = totalMemGibibytes; totalDecimalPlaces = 1; }

    double usedMemKibibytes = kb_main_used;
    double usedMemBytes = usedMemKibibytes * 1024;
    double usedMemMebibytes = usedMemKibibytes / 1024;
    double usedMemGibibytes = usedMemMebibytes / 1000;

    std::string usedSuffix;
    int usedDecimalPlaces;
    double finalUsedNumber;

    if (usedMemKibibytes == 0) { usedSuffix = "B"; finalUsedNumber = usedMemBytes; usedDecimalPlaces = 0; }
    else if (usedMemMebibytes == 0) { usedSuffix = "K"; finalUsedNumber = usedMemKibibytes; usedDecimalPlaces = 0; }
    else if (usedMemGibibytes == 0) { usedSuffix = "M"; finalUsedNumber = usedMemMebibytes; usedDecimalPlaces = 1; }
    else { usedSuffix = "G"; finalUsedNumber = usedMemGibibytes; usedDecimalPlaces = 1; }

    std::ostringstream stringStream;
    stringStream << std::fixed << std::setprecision(usedDecimalPlaces) << finalUsedNumber << usedSuffix << " / " << std::setprecision(totalDecimalPlaces) << finalTotalNumber << totalSuffix;
    return stringStream.str();
}

std::string makeLine(std::string text) {
    std::string line = "";
    char* lineTextEnvVar = getenv("QF_LINETEXT");
    std::string lineText = lineTextEnvVar ? std::string(lineTextEnvVar) : "─";

    for (unsigned int i = 0; i < text.length(); i++) {
        line.append(lineText);
    }
    return line;
}

int main() {meminfo();
    struct utsname un;
    uname(&un);

    passwd* pw = getpwuid(geteuid());
    
    struct sysinfo sysInfo;
    sysinfo(&sysInfo);

    std::string hostname = un.nodename;
    std::string username = pw->pw_name;
    std::string distro = getDistro();
    std::string kernel = std::string(un.sysname) + " " + std::string(un.release);
    std::string uptime = getUptime(sysInfo);
    std::string shell = getShell(pw);

    std::string userInfo = bold(magenta(username)) + bold(green("@")) + bold(magenta(hostname));
    std::cout << userInfo << std::endl;
    std::cout << bold(makeLine(username + "@" + hostname)) << std::endl; // we do this so the ansi doesn't effect the length calculation
    std::cout << bold(red(getSymbol("  ", "os     "))) << distro << std::endl;
    std::cout << bold(yellow(getSymbol("  ", "kernel "))) << kernel << std::endl;
    std::cout << bold(green(getSymbol("  ", "uptime "))) << uptime << std::endl;
    std::cout << bold(cyan(getSymbol("  ", "shell  "))) << shell << std::endl;
    std::cout << bold(magenta(getSymbol("  ", "ram    "))) << getMemory() << std::endl;
}