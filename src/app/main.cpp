#include <iostream>
#include <fstream>

#include <Expert.h>
#include <httplib/httplib.h>

constexpr int SERVICE_PORT_NUMBER = 8787;
constexpr const char* OUTPUT_FILES_DIRECTORY_CONTENT = "../../../../../../creativity-lab/geometric-exercises/content/";
constexpr const char* OUTPUT_FILES_DIRECTORY_PARSED = "../../../../../../creativity-lab/geometric-exercises/parsed/";

int main() {
  httplib::Server server;

  std::cout << "Compute Service Started..." << std::endl;

  server.Options("/(.*)", [&](const httplib::Request& /*req*/, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    res.set_header("Content-Type", "text/html; charset=utf-8");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authorization");
    res.set_header("Access-Control-Allow-Origin", " * ");
    res.set_header("Connection", "close");
  });

  server.Post("/compute", [](const httplib::Request& req, httplib::Response& res) {
    try {
      const std::string inputString = req.body;
      json input = json::parse(inputString)["json"];

      expert::Expert expert;
      expert.importTask(input);
      //expert.useKnowledge();
      json output = expert.exportSolution();

      std::string exerciseName;
      std::cin >> exerciseName;

      std::string target;
      std::cin >> target;

      std::ofstream exerciseContent(OUTPUT_FILES_DIRECTORY_CONTENT + exerciseName + ".txt");
      std::ofstream exerciseParsed(OUTPUT_FILES_DIRECTORY_PARSED + exerciseName + ".txt");

      input["target"] = target;
      output["target"] = target;

      exerciseContent << std::setw(1) << input;
      exerciseParsed  << std::setw(1) << output;

      exerciseContent.close();
      exerciseParsed.close();

      res.set_header("Access-Control-Allow-Origin", " * ");
      res.set_header("Access-Control-Allow-Credentials", "true");
      res.set_header("Access-Control-Allow-Methods", "GET,HEAD,PUT,PATCH,POST,DELETE");
      res.set_header("Access-Control-Expose-Headers", "Content-Length");
      res.set_header("Access-Control-Allow-Headers", "Accept, Authorization, Content-Type, X-Requested-With, Range");

      res.set_content(output.dump(), "application/json");
    } catch (...) {
      res.set_content("Invalid Input!", "text/plain");
    }
  });

  server.listen("localhost", SERVICE_PORT_NUMBER);

  return 0;
}
