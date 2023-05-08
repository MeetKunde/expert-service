#include <iostream>

#include <Expert.h>
#include <httplib/httplib.h>

constexpr int SERVICE_PORT_NUMBER = 8787;

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
      const json input = json::parse(inputString)["json"];

      expert::Expert expert;
      expert.importTask(input);
      expert.useKnowledge();
      const json output = expert.exportSolution();

      res.set_header("Access-Control-Allow-Origin", " * ");
      res.set_header("Access-Control-Allow-Credentials", "true");
      res.set_header("Access-Control-Allow-Methods", "GET,HEAD,PUT,PATCH,POST,DELETE");
      res.set_header("Access-Control-Expose-Headers", "Content-Length");
      res.set_header("Access-Control-Allow-Headers", "Accept, Authorization, Content-Type, X-Requested-With, Range");

      res.set_content(output.dump(), "application/json");
    } catch (const std::exception& exception) {
      std::cout << exception.what() << std::endl;
      res.set_content("Invalid Input!", "text/plain");
    }
  });

  server.listen("localhost", SERVICE_PORT_NUMBER);

  return 0;
}
