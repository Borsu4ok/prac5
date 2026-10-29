#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char* argv[])
{
    const std::string inputPath = argc > 1 ? argv[1] : "input.txt";
    const std::string outputPath = argc > 2 ? argv[2] : "output.txt";

    std::ifstream in(inputPath);
    if (!in) {
        std::cerr << "Cannot open input file: " << inputPath << '\n';
        return 1;
    }
    std::ofstream out(outputPath);
    if (!out) {
        std::cerr << "Cannot open output file: " << outputPath << '\n';
        return 1;
    }

    const std::string sep = R"re(\s*[;:?]\s*)re";
    const std::string surname = R"re(([A-Za-z'-]{1,22}))re";
    const std::string subject = R"re(([A-Za-z"-](?:[A-Za-z" -]{0,54}[A-Za-z"-])?))re";
    const std::string group = R"re(([A-Za-z0-9-]{1,7}))re";
    const std::string patronym = R"re(([A-Za-z'-]{1,22}))re";
    const std::string name = R"re(([A-Za-z'-]{1,20}))re";
    const std::string grade = R"re((-1|0|[2-5]))re";
    const std::string book = R"re((\d{8}))re";
    const std::string total = R"re((0*(?:100|\d{1,2})))re";
    const std::string exam = R"re((\d+))re";

    const std::regex recordRe(
        R"re(^\s*)re" + surname + sep + subject + sep + group + sep + patronym + sep + name +
        sep + grade + sep + book + sep + total + sep + exam + R"re(\s*$)re");
    const std::regex highTotalRe(R"re(0*(?:100|9\d))re");
    const std::regex blankRe(R"re(\s*)re");

    const std::string outFormat = "$1;$2;$3;$4;$5;$6;$7;$8;$9\n";

    std::string line;
    std::smatch m;
    std::size_t lineNo = 0, written = 0, invalid = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        if (std::regex_match(line, blankRe))
            continue;
        if (!std::regex_match(line, m, recordRe)) {
            ++invalid;
            std::cerr << "Line " << lineNo << ": invalid record, skipped\n";
            continue;
        }
        if (std::regex_match(m[8].first, m[8].second, highTotalRe)) {
            out << m.format(outFormat);
            ++written;
        }
    }

    std::cout << "Processed lines: " << lineNo << '\n'
        << "Written records: " << written << '\n'
        << "Invalid records: " << invalid << '\n';
    return 0;
}