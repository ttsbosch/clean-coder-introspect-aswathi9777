#include "TradeProcessor.h"

std::vector<std::string> Processor::SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool Processor::CheckIntegerValidity(const std::string& str, int& value) {
    try {
        value = std::stoi(str);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool Processor::CheckDoubleValidity(const std::string& str, double& value) {
    try {
        value = std::stod(str);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool Processor::ProcessLine(const std::string& line, TR& trade, int lineNumber) {
    std::vector<std::string> fields = SplitString(line, ',');

    if (fields.size() != 3) {
        std::cout << "WARN: Line " << lineNumber << " malformed. Only " << fields.size() << " field(s) found." << std::endl;
        return false;
    }

    if (fields[0].length() != 6) {
        std::cout << "WARN: Trade currencies on line " << lineNumber << " malformed: '" << fields[0] << "'" << std::endl;
        return false;
    }

    int tradeAmount;
    if (!CheckIntegerValidity(fields[1], tradeAmount)) {
        std::cout << "WARN: Trade amount on line " << lineNumber << " not a valid integer: '" << fields[1] << "'" << std::endl;
        return false;
    }

    double tradePrice;
    if (!CheckDoubleValidity(fields[2], tradePrice)) {
        std::cout << "WARN: Trade price on line " << lineNumber << " not a valid decimal: '" << fields[2] << "'" << std::endl;
        return false;
    }

    trade.SC = fields[0].substr(0, 3);
    trade.DC = fields[0].substr(3, 3);
    trade.Lots = static_cast<float>(tradeAmount) / LotSize;
    trade.Price = tradePrice;

    return true;
}

std::string Processor::GenerateXML(const std::vector<TR>& trades) {
    std::ostringstream xmlStream;
    xmlStream << "<TradeRecords>" << std::endl;

    for (const auto& trade : trades) {
        xmlStream << "\t<TradeRecord>" << std::endl;
        xmlStream << "\t\t<SourceCurrency>" << trade.SC << "</SourceCurrency>" << std::endl;
        xmlStream << "\t\t<DestinationCurrency>" << trade.DC << "</DestinationCurrency>" << std::endl;
        xmlStream << "\t\t<Lots>" << trade.Lots << "</Lots>" << std::endl;
        xmlStream << "\t\t<Price>" << trade.Price << "</Price>" << std::endl;
        xmlStream << "\t</TradeRecord>" << std::endl;
    }

    xmlStream << "</TradeRecords>";
    return xmlStream.str();
}

void Processor::Process(std::istream& stream) {
    std::vector<std::string> lines;
    std::string line;

    // Read lines from the input stream
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    std::vector<TR> trades;
    int lineNumber = 1;

    // Process each line
    for (const auto& line : lines) {
        TR trade;
        if (ProcessLine(line, trade, lineNumber)) {
            trades.push_back(trade);
        }
        lineNumber++;
    }

    // Generate and write XML data to file
    std::string xmlData = GenerateXML(trades);
    std::ofstream outFile("output.xml");
    outFile << xmlData;

    std::cout << "INFO: " << trades.size() << " trades processed" << std::endl;
}
