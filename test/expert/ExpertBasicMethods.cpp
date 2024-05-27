#include "ExpertTestBase.h"

size_t filterObjectPos(const json& objects, const std::string& identifier) {
  for(const auto& obj: objects) {
    if(obj["object"]["id"] == identifier) {
      return obj["position"];
    }
  }

  return 0;
}

json filterDependenciesByType(const json& dependencies, DependencyType type) {
  for(const auto& dependenciesGroup: dependencies) {
    if(dependenciesGroup["type"] == type) {
      return dependenciesGroup["dependencies"];
    }
  }

  return json{};
}

TEST_CASE("Empty expert", "[expert]") {
  const json emptyInputJson{};
  const json emptyOutputJson{
      {"points", json{}},
      {"lines", json{}},
      {"circles", json{}},
      {"intersections", {
        {"circle_circle", std::vector<json>{}},
        {"line_circle", std::vector<json>{}},
        {"line_line", std::vector<json>{}},
        {"points_on_circle_circle", std::vector<json>{}},
        {"points_on_line_circle", std::vector<json>{}},
        {"points_on_line_line", std::vector<json>{}},
      }},
      {"indexes_of_variables", json{}},
      {"dependencies", {
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::SEGMENT_LENGTH }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::ANGLE_MEASURE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::EQUATION }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::POLYGON_TYPE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::POLYGON_PERIMETER }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::POLYGON_AREA }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::EQUAL_SEGMENTS }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::EQUAL_ANGLES }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::PERPENDICULAR_LINES }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::PARALLEL_LINES }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::TANGENT_LINE_TO_CIRCLE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::TANGENT_CIRCLE_TO_CIRCLE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::BISECTOR_LINE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::MID_PERPENDICULAR_LINE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::INSCRIBED_CIRCLE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::CIRCUMSCRIBED_CIRCLE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::ESCRIBED_CIRCLE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::MEDIAN }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::ALTITUDE }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::MID_SEGMENT }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::SIMILAR_TRIANGLES }},
        {{"dependencies", std::vector<json>{}}, {"type", DependencyType::CONGRUENT_TRIANGLES }}
      }},
  };

  Expert expert{};

  REQUIRE_NOTHROW(expert.importTask(emptyInputJson));
  REQUIRE_NOTHROW(expert.useKnowledge());
  REQUIRE(expert.exportSolution() == emptyOutputJson);
}

TEST_CASE("Importing task(shapes part) and exporting empty solution from expert", "[expert]") {
  // Testing shapes importing and preprocessing
  const json task1 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":72.79604861610814,\"y\":48.026315789473685,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":98.0592225727282,\"y\":64.60526115015934,\"name\":\"B\"},{\"id\":\"jxgBoard1P25\",\"x\":101.7434371145148,\"y\":16.18420450310958,\"name\":\"C\"},{\"id\":\"jxgBoard1P27\",\"x\":121.21710526315789,\"y\":-5.657898752312911,\"name\":\"D\"},{\"id\":\"jxgBoard1P34\",\"x\":34.638153879266035,\"y\":-51.71053033126028,\"name\":\"G\"},{\"id\":\"jxgBoard1P36\",\"x\":57.26974888851768,\"y\":-59.868421052631575,\"name\":\"H\"},{\"id\":\"jxgBoard1P47\",\"x\":-33.78289875231291,\"y\":-59.868421052631575,\"name\":\"M\"},{\"id\":\"jxgBoard1P62\",\"x\":-33.78289875231291,\"y\":-5.657898752312911,\"name\":\"T\"},{\"id\":\"jxgBoard1P79\",\"x\":-112.20394937615646,\"y\":8.289467661004318,\"name\":\"B'\"},{\"id\":\"jxgBoard1P81\",\"x\":61.48027118883635,\"y\":57.76315488313374,\"name\":\"C'\"},{\"id\":\"jxgBoard1P110\",\"x\":-122.99341904489617,\"y\":74.60525914242393,\"name\":\"Q'\"},{\"id\":\"jxgBoard1P100\",\"x\":-89.57236440558182,\"y\":-59.868421052631575,\"name\":\"L'\"},{\"id\":\"jxgBoard1P102\",\"x\":110.69078947368419,\"y\":-59.868421052631575,\"name\":\"M'\"},{\"id\":\"jxgBoard1P104\",\"x\":129.504398695776,\"y\":25.437865285078733,\"name\":\"N'\"},{\"id\":\"jxgBoard1P106\",\"x\":-33.78289875231291,\"y\":64.60526115015934,\"name\":\"O'\"},{\"id\":\"jxgBoard1P108\",\"x\":-89.88415967309535,\"y\":14.647225959604285,\"name\":\"P'\"},{\"id\":\"jxgBoard1P30\",\"x\":102.90114654530112,\"y\":45.42391952665055,\"name\":\"E\"},{\"id\":\"jxgBoard1P32\",\"x\":72.52601908889174,\"y\":17.810153364548125,\"name\":\"F\"},{\"id\":\"jxgBoard1P60\",\"x\":12.006558870014377,\"y\":-59.868421052631575,\"name\":\"S\"},{\"id\":\"jxgBoard1P86\",\"x\":42.90761428954584,\"y\":52.47276168949537,\"name\":\"E'\"},{\"id\":\"jxgBoard1P98\",\"x\":-33.78289875231291,\"y\":30.627585180108458,\"name\":\"K'\"}],\"lines\":[{\"id\":\"jxgBoard1L83\",\"a\":0.2848484857242188,\"b\":40.25059273307939,\"type\":2,\"pointsOn\":[\"jxgBoard1P79\",\"jxgBoard1P81\",\"jxgBoard1P86\",\"jxgBoard1P98\",\"jxgBoard1P108\"]},{\"id\":\"jxgBoard1L64\",\"a\":0,\"b\":33.78289875231291,\"type\":0,\"pointsOn\":[\"jxgBoard1P47\",\"jxgBoard1P62\",\"jxgBoard1P98\",\"jxgBoard1P106\"]},{\"id\":\"jxgBoard1L49\",\"a\":0,\"b\":-59.868421052631575,\"type\":1,\"pointsOn\":[\"jxgBoard1P47\",\"jxgBoard1P36\",\"jxgBoard1P60\",\"jxgBoard1P100\",\"jxgBoard1P102\"]}],\"circles\":[{\"id\":\"jxgBoard1C24\",\"cx\":72.79604861610814,\"cy\":48.026315789473685,\"r\":30.217368972745568,\"centerId\":\"jxgBoard1P20\",\"pointsOn\":[\"jxgBoard1P22\",\"jxgBoard1P30\",\"jxgBoard1P32\",\"jxgBoard1P86\"]},{\"id\":\"jxgBoard1C29\",\"cx\":101.7434371145148,\"cy\":16.18420450310958,\"r\":29.262625066524944,\"centerId\":\"jxgBoard1P25\",\"pointsOn\":[\"jxgBoard1P27\",\"jxgBoard1P30\",\"jxgBoard1P32\",\"jxgBoard1P104\"]},{\"id\":\"jxgBoard1C38\",\"cx\":34.638153879266035,\"cy\":-51.71053033126028,\"r\":24.05702129700641,\"centerId\":\"jxgBoard1P34\",\"pointsOn\":[\"jxgBoard1P36\",\"jxgBoard1P60\"]}],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[],\"angleMeasures\":[],\"formulas\":[]}");

  const json expectedPoints{
    {{"object", {{"id","jxgBoard1P20"}, {"name","A"}, {"x",72.79605102539063}, {"y",48.02631759643555}}}, {"position",0}},
    {{"object", {{"id","jxgBoard1P22"}, {"name","B"}, {"x",98.05921936035156}, {"y",64.60526275634766}}}, {"position",1}},
    {{"object", {{"id","jxgBoard1P25"}, {"name","C"}, {"x",101.74343872070313}, {"y",16.1842041015625}}}, {"position",2}},
    {{"object", {{"id","jxgBoard1P27"}, {"name","D"}, {"x",121.21710205078125}, {"y",-5.657898902893066}}}, {"position",3}},
    {{"object", {{"id","jxgBoard1P34"}, {"name","G"}, {"x",34.638153076171875}, {"y",-51.71052932739258}}}, {"position",4}},
    {{"object", {{"id","jxgBoard1P36"}, {"name","H"}, {"x",57.26974868774414}, {"y",-59.8684196472168}}}, {"position",5}},
    {{"object", {{"id","jxgBoard1P47"}, {"name","M"}, {"x",-33.78289794921875}, {"y",-59.8684196472168}}}, {"position",6}},
    {{"object", {{"id","jxgBoard1P62"}, {"name","T"}, {"x",-33.78289794921875}, {"y",-5.657898902893066}}}, {"position",7}},
    {{"object", {{"id","jxgBoard1P79"}, {"name","B'"}, {"x",-112.20394897460938}, {"y",8.289467811584473}}}, {"position",8}},
    {{"object", {{"id","jxgBoard1P81"}, {"name","C'"}, {"x",61.48027038574219}, {"y",57.763153076171875}}}, {"position",9}},
    {{"object", {{"id","jxgBoard1P110"}, {"name","Q'"}, {"x",-122.99341583251953}, {"y",74.60526275634766}}}, {"position",10}},
    {{"object", {{"id","jxgBoard1P100"}, {"name","L'"}, {"x",-89.5723648071289}, {"y",-59.8684196472168}}}, {"position",11}},
    {{"object", {{"id","jxgBoard1P102"}, {"name","M'"}, {"x",110.69078826904297}, {"y",-59.8684196472168}}}, {"position",12}},
    {{"object", {{"id","jxgBoard1P104"}, {"name","N'"}, {"x",129.50439453125}, {"y",25.4378662109375}}}, {"position",13}},
    {{"object", {{"id","jxgBoard1P106"}, {"name","O'"}, {"x",-33.78289794921875}, {"y",64.60526275634766}}}, {"position",14}},
    {{"object", {{"id","jxgBoard1P108"}, {"name","P'"}, {"x",-89.88416290283203}, {"y",14.647226333618164}}}, {"position",15}},
    {{"object", {{"id","jxgBoard1P30"}, {"name","E"}, {"x",102.9011459350586}, {"y",45.423919677734375}}}, {"position",16}},
    {{"object", {{"id","jxgBoard1P32"}, {"name","F"}, {"x",72.52601623535156}, {"y",17.81015396118164}}}, {"position",17}},
    {{"object", {{"id","jxgBoard1P60"}, {"name","S"}, {"x",12.006558418273926}, {"y",-59.8684196472168}}}, {"position",18}},
    {{"object", {{"id","jxgBoard1P86"}, {"name","E'"}, {"x",42.907615661621094}, {"y",52.47276306152344}}}, {"position",19}},
    {{"object", {{"id","jxgBoard1P98"}, {"name","K'"}, {"x",-33.78289794921875}, {"y",30.62758445739746}}}, {"position",20}}
  };

  const json expectedLines{
    {{"object", {{"a", 0.28484848141670227}, {"b",40.25059127807617}, {"end1Id","jxgBoard1P79"}, {"end2Id","jxgBoard1P81"}, {"id","jxgBoard1L83"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P79","jxgBoard1P108","jxgBoard1P98","jxgBoard1P86","jxgBoard1P81"})}, {"type",DependencyType::EQUATION}}}, {"position",0}},
    {{"object", {{"a", 0.0}, {"b",33.78289794921875}, {"end1Id","jxgBoard1P47"}, {"end2Id","jxgBoard1P106"}, {"id","jxgBoard1L64"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P47","jxgBoard1P62","jxgBoard1P98","jxgBoard1P106"})}, {"type",DependencyType::SEGMENT_LENGTH}}}, {"position",1}},
    {{"object", {{"a", 0.0}, {"b",-59.8684196472168}, {"end1Id","jxgBoard1P100"}, {"end2Id","jxgBoard1P102"}, {"id","jxgBoard1L49"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P100","jxgBoard1P47","jxgBoard1P60","jxgBoard1P36","jxgBoard1P102"})}, {"type",DependencyType::ANGLE_MEASURE}}}, {"position", 2}}
  };

  const json expectedCircles{
    {{"object", {{"centerId","jxgBoard1P20"}, {"cx",72.79605102539063}, {"cy",48.02631759643555}, {"id","jxgBoard1C24"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P86","jxgBoard1P32","jxgBoard1P30","jxgBoard1P22"})}, {"r",30.217369079589844}}}, {"position",0}},
    {{"object", {{"centerId","jxgBoard1P25"}, {"cx",101.74343872070313}, {"cy",16.1842041015625}, {"id","jxgBoard1C29"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P32","jxgBoard1P27","jxgBoard1P104","jxgBoard1P30"})}, {"r",29.262624740600586}}},{"position",1}},
    {{"object", {{"centerId","jxgBoard1P34"}, {"cx",34.638153076171875}, {"cy",-51.71052932739258}, {"id","jxgBoard1C38"}, {"pointsOn", json(std::vector<std::string>{"jxgBoard1P60","jxgBoard1P36"})}, {"r",24.057022094726563}}}, {"position",2}}
  };

  std::vector<std::vector<std::vector<size_t>>> expectedLineLineIntersections{
      { { }, { }, { } },
      { { }, { }, { } },
      { { }, { }, { } }
  };

  std::vector<std::vector<std::pair<size_t, size_t>>> expectedPointsOnLineLine{};
  expectedPointsOnLineLine.resize(expectedPoints.size(), {});

  for(const auto& line1: expectedLines) {
    for(const auto& line2: expectedLines) {
      if(line1["position"] >= line2["position"]) {
        continue;
      }

      for(const auto& point1Id: line1["object"]["pointsOn"]) {
        for(const auto& point2Id: line2["object"]["pointsOn"]) {
          if(point1Id == point2Id) {
            const size_t pos = filterObjectPos(expectedPoints, point1Id);
            expectedLineLineIntersections[line1["position"]][line2["position"]].push_back(pos);
            expectedLineLineIntersections[line2["position"]][line1["position"]].push_back(pos);
            expectedPointsOnLineLine[pos].push_back({line1["position"], line2["position"]});
          }
        }
      }
    }
  }

  std::vector<std::vector<std::vector<size_t>>> expectedCircleCircleIntersections{
      { { }, { }, { } },
      { { }, { }, { } },
      { { }, { }, { } }
  };

  std::vector<std::vector<std::pair<size_t, size_t>>> expectedPointsOnCircleCircle{};
  expectedPointsOnCircleCircle.resize(expectedPoints.size(), {});

  for(const auto& circle1: expectedCircles) {
    for(const auto& circle2: expectedCircles) {
      if(circle1["position"] >= circle2["position"]) {
        continue;
      }

      for(const auto& point1Id: circle1["object"]["pointsOn"]) {
        for(const auto& point2Id: circle2["object"]["pointsOn"]) {
          if(point1Id == point2Id) {
            const size_t pos = filterObjectPos(expectedPoints, point1Id);
            expectedCircleCircleIntersections[circle1["position"]][circle2["position"]].push_back(pos);
            expectedCircleCircleIntersections[circle2["position"]][circle1["position"]].push_back(pos);
            expectedPointsOnCircleCircle[pos].push_back({circle1["position"], circle2["position"]});
          }
        }
      }
    }
  }

  std::vector<std::vector<std::vector<size_t>>> expectedLineCircleIntersections{
      { { }, { }, { } },
      { { }, { }, { } },
      { { }, { }, { } }
  };

  std::vector<std::vector<std::pair<size_t, size_t>>> expectedPointsOnLineCircle{};
  expectedPointsOnLineCircle.resize(expectedPoints.size(), {});

  for(const auto& line: expectedLines) {
    for(const auto& circle: expectedCircles) {
      for(const auto& point1Id: line["object"]["pointsOn"]) {
        for(const auto& point2Id: circle["object"]["pointsOn"]) {
          if(point1Id == point2Id) {
            const size_t pos = filterObjectPos(expectedPoints, point1Id);
            expectedLineCircleIntersections[line["position"]][circle["position"]].push_back(pos);
            expectedPointsOnLineCircle[pos].push_back({line["position"], circle["position"]});
          }
        }
      }
    }
  }

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task1));
  const json solution = expert.exportSolution();

  REQUIRE(solution["points"] == expectedPoints);
  REQUIRE(solution["lines"] == expectedLines);
  REQUIRE(solution["circles"] == expectedCircles);
  REQUIRE(solution["intersections"]["line_line"] == json(expectedLineLineIntersections));
  REQUIRE(solution["intersections"]["points_on_line_line"] == json(expectedPointsOnLineLine));
  REQUIRE(solution["intersections"]["circle_circle"] == json(expectedCircleCircleIntersections));
  REQUIRE(solution["intersections"]["points_on_circle_circle"] == json(expectedPointsOnCircleCircle));
  REQUIRE(solution["intersections"]["line_circle"] == json(expectedLineCircleIntersections));
  REQUIRE(solution["intersections"]["points_on_line_circle"] == json(expectedPointsOnLineCircle));
}

TEST_CASE("Importing task(part 1) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Segment Length
  //  -> Angle Measure
  //  -> Formula
  const json task2 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-81.09325151351487,\"y\":42.443729903536976,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-35.56269311444936,\"y\":-9.260456048407356,\"name\":\"B\"},{\"id\":\"jxgBoard1P24\",\"x\":2.2508038585209005,\"y\":27.524117718172228,\"name\":\"C\"}],\"lines\":[],\"circles\":[],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[{\"segmentEnd1Id\":\"jxgBoard1P20\",\"segmentEnd2Id\":\"jxgBoard1P24\",\"length\":\"5*x\"}],\"angleMeasures\":[{\"angleEnd1Id\":\"jxgBoard1P20\",\"angleVertexId\":\"jxgBoard1P22\",\"angleEnd2Id\":\"jxgBoard1P24\",\"angleIsConvex\":true,\"measure\":\"50\"}],\"formulas\":[\"x = a/3\",\"|AC| - 7 = x\"]}");

  const json expectedIndexesOfVariables{
      {{"ids", std::vector<size_t>{0, 0}}, {"name", "|AA|"}},
      {{"ids", std::vector<size_t>{1, 1}}, {"name", "|BB|"}},
      {{"ids", std::vector<size_t>{2, 2}}, {"name", "|CC|"}},
      {{"ids", std::vector<size_t>{0, 1}}, {"name", "|AB|"}},
      {{"ids", std::vector<size_t>{0, 2}}, {"name", "|AC|"}},
      {{"ids", std::vector<size_t>{1, 2}}, {"name", "|BC|"}},
      
      {{ "ids", std::vector<size_t>{0, 0, 0, 1}}, {"name", "|>AAA|"}},
      {{ "ids", std::vector<size_t>{0, 0, 0, 0}}, {"name", "|<AAA|"}},
      {{ "ids", std::vector<size_t>{1, 1, 1, 1}}, {"name", "|>BBB|"}},
      {{ "ids", std::vector<size_t>{1, 1, 1, 0}}, {"name", "|<BBB|"}},
      {{ "ids", std::vector<size_t>{2, 2, 2, 1}}, {"name", "|>CCC|"}},
      {{ "ids", std::vector<size_t>{2, 2, 2, 0}}, {"name", "|<CCC|"}},
      
      {{ "ids", std::vector<size_t>{0, 0, 2, 1}}, {"name","|>AAC|" }},
      {{ "ids", std::vector<size_t>{0, 0, 2, 0}}, {"name","|<AAC|" }},
      {{ "ids", std::vector<size_t>{0, 2, 0, 1}}, {"name","|>ACA|" }},
      {{ "ids", std::vector<size_t>{0, 2, 0, 0}}, {"name","|<ACA|" }},
      {{ "ids", std::vector<size_t>{0, 0, 1, 1}}, {"name","|>AAB|" }},
      {{ "ids", std::vector<size_t>{0, 0, 1, 0}}, {"name","|<AAB|" }},
      {{ "ids", std::vector<size_t>{0, 1, 0, 1}}, {"name","|>ABA|" }},
      {{ "ids", std::vector<size_t>{0, 1, 0, 0}}, {"name","|<ABA|" }},
      {{ "ids", std::vector<size_t>{0, 1, 1, 1}}, {"name","|>ABB|" }},
      {{ "ids", std::vector<size_t>{0, 1, 1, 0}}, {"name","|<ABB|" }},
      {{ "ids", std::vector<size_t>{1, 0, 1, 1}}, {"name","|>BAB|" }},
      {{ "ids", std::vector<size_t>{1, 0, 1, 0}}, {"name","|<BAB|" }},
      {{ "ids", std::vector<size_t>{1, 1, 2, 1}}, {"name","|>BBC|" }},
      {{ "ids", std::vector<size_t>{1, 1, 2, 0}}, {"name","|<BBC|" }},
      {{ "ids", std::vector<size_t>{1, 2, 1, 1}}, {"name","|>BCB|" }},
      {{ "ids", std::vector<size_t>{1, 2, 1, 0}}, {"name","|<BCB|" }},
      {{ "ids", std::vector<size_t>{0, 2, 2, 1}}, {"name","|>ACC|" }},
      {{ "ids", std::vector<size_t>{0, 2, 2, 0}}, {"name","|<ACC|" }},
      {{ "ids", std::vector<size_t>{2, 0, 2, 1}}, {"name","|>CAC|" }},
      {{ "ids", std::vector<size_t>{2, 0, 2, 0}}, {"name","|<CAC|" }},
      {{ "ids", std::vector<size_t>{1, 2, 2, 1}}, {"name","|>BCC|" }},
      {{ "ids", std::vector<size_t>{1, 2, 2, 0}}, {"name","|<BCC|" }},
      {{ "ids", std::vector<size_t>{2, 1, 2, 1}}, {"name","|>CBC|" }},
      {{ "ids", std::vector<size_t>{2, 1, 2, 0}}, {"name","|<CBC|" }},

      {{ "ids", std::vector<size_t>{1, 0, 2, 1}}, {"name", "|>BAC|"}},
      {{ "ids", std::vector<size_t>{1, 0, 2, 0}}, {"name", "|<BAC|"}},
      {{ "ids", std::vector<size_t>{0, 1, 2, 1}}, {"name", "|>ABC|"}},
      {{ "ids", std::vector<size_t>{0, 1, 2, 0}}, {"name", "|<ABC|"}},
      {{ "ids", std::vector<size_t>{0, 2, 1, 1}}, {"name", "|>ACB|"}},
      {{ "ids", std::vector<size_t>{0, 2, 1, 0}}, {"name", "|<ACB|"}}
  };

  const json expectedLengths = {
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",0}, {"importance",0}, {"object1",{{"value","|AA|"}, {"variables",json(std::vector<std::string>{"|AA|"})}}}, {"object2",{{"value","0"}, {"variables", json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::SEGMENT_LENGTH}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",3}, {"importance",0}, {"object1",{{"value","|BB|"}, {"variables",json(std::vector<std::string>{"|BB|"})}}}, {"object2",{{"value","0"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::SEGMENT_LENGTH}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",6}, {"importance",0}, {"object1",{{"value","|CC|"}, {"variables",json(std::vector<std::string>{"|CC|"})}}}, {"object2",{{"value","0"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::SEGMENT_LENGTH}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",21}, {"importance",2}, {"object1",{{"value","|AC|"}, {"variables",json(std::vector<std::string>{"|AC|"})}}}, {"object2",{{"value","5*x"}, {"variables",json(std::vector<std::string>{"x"})}}}, {"reason",DependencyReason::USER_DEFINED}, {"type",DependencyType::SEGMENT_LENGTH}}
  };

  const json expectedMeasures = {
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",1}, {"importance",0}, {"object1",{{"value","|>AAA|"}, {"variables",json(std::vector<std::string>{"|>AAA|"})}}}, {"object2",{{"value","0"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",2}, {"importance",0}, {"object1",{{"value","|<AAA|"}, {"variables",json(std::vector<std::string>{"|<AAA|"})}}}, {"object2",{{"value","360"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",4}, {"importance",0}, {"object1",{{"value","|>BBB|"}, {"variables",json(std::vector<std::string>{"|>BBB|"})}}}, {"object2",{{"value","0"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",5}, {"importance",0}, {"object1",{{"value","|<BBB|"}, {"variables",json(std::vector<std::string>{"|<BBB|"})}}}, {"object2",{{"value","360"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",7}, {"importance",0}, {"object1",{{"value","|>CCC|"}, {"variables",json(std::vector<std::string>{"|>CCC|"})}}}, {"object2",{{"value","0"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",8}, {"importance",0}, {"object1",{{"value","|<CCC|"}, {"variables",json(std::vector<std::string>{"|<CCC|"})}}}, {"object2",{{"value","360"}, {"variables",json(std::vector<std::string>{})}}}, {"reason",DependencyReason::POINTS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",9}, {"importance",0}, {"object1",{{"value","|>BAB|"}, {"variables",json(std::vector<std::string>{"|>BAB|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",10}, {"importance",0}, {"object1",{{"value","|<BAB|"}, {"variables",json(std::vector<std::string>{"|<BAB|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",11}, {"importance",0}, {"object1",{{"value","|>CAC|"}, {"variables",json(std::vector<std::string>{"|>CAC|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",12}, {"importance",0}, {"object1",{{"value","|<CAC|"}, {"variables",json(std::vector<std::string>{"|<CAC|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",13}, {"importance",0}, {"object1",{{"value","|>ABA|"}, {"variables",json(std::vector<std::string>{"|>ABA|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",14}, {"importance",0}, {"object1",{{"value","|<ABA|"}, {"variables",json(std::vector<std::string>{"|<ABA|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",15}, {"importance",0}, {"object1",{{"value","|>CBC|"}, {"variables",json(std::vector<std::string>{"|>CBC|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",16}, {"importance",0}, {"object1",{{"value","|<CBC|"}, {"variables",json(std::vector<std::string>{"|<CBC|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",17}, {"importance",0}, {"object1",{{"value","|>ACA|"}, {"variables",json(std::vector<std::string>{"|>ACA|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",18}, {"importance",0}, {"object1",{{"value","|<ACA|"}, {"variables",json(std::vector<std::string>{"|<ACA|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",19}, {"importance",0}, {"object1",{{"value","|>BCB|"}, {"variables",json(std::vector<std::string>{"|>BCB|"})}}}, {"object2",{{"value","0"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",20}, {"importance",0}, {"object1",{{"value","|<BCB|"}, {"variables",json(std::vector<std::string>{"|<BCB|"})}}}, {"object2",{{"value","360"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::ARMS_ARE_THE_SAME}, {"type",DependencyType::ANGLE_MEASURE}},
      {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",22}, {"importance",2}, {"object1",{{"value","|>ABC|"}, {"variables",json(std::vector<std::string>{"|>ABC|"})}}}, {"object2",{{"value","50"},{"variables",json(std::vector<size_t>{})}}}, {"reason",DependencyReason::USER_DEFINED} ,{"type",DependencyType::ANGLE_MEASURE}}
  };

  const json expectedFormulas{
    {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",23}, {"importance",2}, {"object1",{{"value","a*3^-1"}, {"variables", json(std::vector<std::string>{"a"})}}}, {"object2",{{"value","x"}, {"variables", json(std::vector<std::string>{"x"})}}}, {"reason",DependencyReason::USER_DEFINED}, {"type",DependencyType::EQUATION}},
    {{"basedOn",json(std::vector<size_t>{})}, {"category",0}, {"id",24}, {"importance",2}, {"object1",{{"value","x"}, {"variables", json(std::vector<std::string>{"x"})}}}, {"object2",{{"value","|AC|-7"}, {"variables", json(std::vector<std::string>{"|AC|"})}}}, {"reason",DependencyReason::USER_DEFINED}, {"type",DependencyType::EQUATION}}
  };

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task2));
  const json solution = expert.exportSolution();

  REQUIRE(expectedIndexesOfVariables.size() == solution["indexes_of_variables"].size());
  for(const auto& var1: expectedIndexesOfVariables) {
    bool found = false;
    for(const auto& var2: solution["indexes_of_variables"]) {
      if(var1 == var2) {
        found = true;
        break;
      }
    }

    REQUIRE(found);
  }

  size_t allDependenciesNumber{0};
  for(const auto& dep: solution["dependencies"]) {
    allDependenciesNumber += dep["dependencies"].size();
  }

  REQUIRE((expectedLengths.size() + expectedMeasures.size() + expectedFormulas.size()) == allDependenciesNumber);

  for(const auto& len: expectedLengths) {
    bool found = false;
    for(const auto& depVec: solution["dependencies"]) {
      for(const auto& dep: depVec["dependencies"]) {
        if(len == dep) {
          found = true;
          break;
        }
      }
    }
    REQUIRE(found);
  }

  for(const auto& meas: expectedMeasures) {
    bool found = false;
    for(const auto& depVec: solution["dependencies"]) {
      for(const auto& dep: depVec["dependencies"]) {
        if(meas == dep) {
          found = true;
          break;
        }
      }
    }
    REQUIRE(found);
  }

  for(const auto& form: expectedFormulas) {
    bool found = false;
    for(const auto& depVec: solution["dependencies"]) {
      for(const auto& dep: depVec["dependencies"]) {
        if(form == dep) {
          found = true;
          break;
        }
      }
    }
    REQUIRE(found);
  }
}

TEST_CASE("Importing task(part 2) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Parallel Lines
  //  -> Perpendicular Lines
  //  -> Mid-Perpendicular
  //  -> Bisector
  //  -> Tangent Circle-Line
  //  -> Tangent Line-Circle
  //  -> Tangent Circle-Circle
  const json task3 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-74.24389846925813,\"y\":-47.121959159044714,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-122.8943069582063,\"y\":1.2682906980436992,\"name\":\"B\"},{\"id\":\"jxgBoard1P25\",\"x\":-84.39023397802337,\"y\":-79.12194724974593,\"name\":\"C\"},{\"id\":\"jxgBoard1P41\",\"x\":-122.8943069582063,\"y\":37.691054925685975,\"name\":\"E\"},{\"id\":\"jxgBoard1P43\",\"x\":-98.82927127000761,\"y\":54.6016190691692,\"name\":\"F\"},{\"id\":\"jxgBoard1P45\",\"x\":-74.24389846925813,\"y\":41.07316676194105,\"name\":\"G\"},{\"id\":\"jxgBoard1P47\",\"x\":2.7642276422764227,\"y\":-58.82927623221544,\"name\":\"H\"},{\"id\":\"jxgBoard1P49\",\"x\":24.35773151676829,\"y\":-35.154493378429876,\"name\":\"I\"},{\"id\":\"jxgBoard1P51\",\"x\":51.41464605564024,\"y\":-58.82927623221544,\"name\":\"J\"},{\"id\":\"jxgBoard1P53\",\"x\":-100.43867351738308,\"y\":83.97001954878812,\"name\":\"K\"},{\"id\":\"jxgBoard1P64\",\"x\":26.15942154004171,\"y\":-67.14712793408395,\"name\":\"L\"},{\"id\":\"jxgBoard1P83\",\"x\":86.53657742632113,\"y\":32.48780785537348,\"name\":\"M\"},{\"id\":\"jxgBoard1P85\",\"x\":110.21137020452235,\"y\":36.39024687976372,\"name\":\"N\"},{\"id\":\"jxgBoard1P91\",\"x\":107.08944677337398,\"y\":-35.154493378429876,\"name\":\"O\"},{\"id\":\"jxgBoard1P93\",\"x\":121.60993039262651,\"y\":-32.761006666227836,\"name\":\"P\"},{\"id\":\"jxgBoard1P96\",\"x\":90.69919493140243,\"y\":-71.31706920096545,\"name\":\"Q\"},{\"id\":\"jxgBoard1P98\",\"x\":101.01427423302208,\"y\":-48.558429434060066,\"name\":\"R\"},{\"id\":\"jxgBoard1P32\",\"x\":-98.5691027137322,\"y\":-22.926834230500514,\"name\":\"D\"}],\"lines\":[{\"id\":\"jxgBoard1L24\",\"a\":-0.9946524882330874,\"b\":-120.96883750761701,\"type\":2,\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P32\"]},{\"id\":\"jxgBoard1L63\",\"a\":-18.248017565224213,\"b\":-1748.8366590242385,\"type\":2,\"pointsOn\":[\"jxgBoard1P43\",\"jxgBoard1P53\"]},{\"id\":\"jxgBoard1L82\",\"a\":-17.75701377172968,\"b\":397.3660806129187,\"type\":2,\"pointsOn\":[\"jxgBoard1P49\",\"jxgBoard1P64\"]},{\"id\":\"jxgBoard1L28\",\"a\":-0.9946524882330874,\"b\":-163.06090345855932,\"type\":2,\"pointsOn\":[\"jxgBoard1P25\"]},{\"id\":\"jxgBoard1L31\",\"a\":1.0053762613879462,\"b\":5.721990684733286,\"type\":2,\"pointsOn\":[\"jxgBoard1P25\"]},{\"id\":\"jxgBoard1L40\",\"a\":1.0053762613879509,\"b\":76.17220174419614,\"type\":2,\"pointsOn\":[\"jxgBoard1P32\"]},{\"id\":\"jxgBoard1L90\",\"a\":-6.066665649417158,\"b\":705.0057806747309,\"type\":2,\"pointsOn\":[\"jxgBoard1P85\"]}],\"circles\":[{\"id\":\"jxgBoard1C87\",\"cx\":86.53657742632113,\"cy\":32.48780785537348,\"r\":23.381368531195893,\"centerId\":\"jxgBoard1P83\",\"pointsOn\":[\"jxgBoard1P85\"]},{\"id\":\"jxgBoard1C95\",\"cx\":107.08944677337398,\"cy\":-35.154493378429876,\"r\":14.716426984104151,\"centerId\":\"jxgBoard1P91\",\"pointsOn\":[\"jxgBoard1P93\"]},{\"id\":\"jxgBoard1C100\",\"cx\":90.69919493140243,\"cy\":-71.31706920096545,\"r\":24.987127586788947,\"centerId\":\"jxgBoard1P96\",\"pointsOn\":[\"jxgBoard1P98\"]}],\"perpendicularLines\":[{\"line1Id\":\"jxgBoard1L31\",\"line2Id\":\"jxgBoard1L24\"}],\"parallelLines\":[{\"line1Id\":\"jxgBoard1L28\",\"line2Id\":\"jxgBoard1L24\"}],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[{\"segmentEnd1Id\":\"jxgBoard1P20\",\"segmentEnd2Id\":\"jxgBoard1P22\",\"lineId\":\"jxgBoard1L40\"}],\"bisectors\":[{\"angleEnd1Id\":\"jxgBoard1P41\",\"angleVertexId\":\"jxgBoard1P43\",\"angleEnd2Id\":\"jxgBoard1P45\",\"angleType\":2,\"lineId\":\"jxgBoard1L63\"},{\"angleEnd1Id\":\"jxgBoard1P47\",\"angleVertexId\":\"jxgBoard1P49\",\"angleEnd2Id\":\"jxgBoard1P51\",\"angleType\":1,\"lineId\":\"jxgBoard1L82\"}],\"tangentLines\":[{\"circleId\":\"jxgBoard1C87\",\"lineId\":\"jxgBoard1L90\"},{\"circleId\":\"jxgBoard1C95\",\"lineId\":\"jxgBoard1L90\"}],\"tangentCircles\":[{\"circle1Id\":\"jxgBoard1C95\",\"circle2Id\":\"jxgBoard1C100\"}],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[],\"angleMeasures\":[],\"formulas\":[]}");

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task3));
  const json solution = expert.exportSolution();
  const json dependencies{solution["dependencies"]};

  const json parallelLines{filterDependenciesByType(dependencies, DependencyType::PARALLEL_LINES)};
  const json perpendicularLines{filterDependenciesByType(dependencies, DependencyType::PERPENDICULAR_LINES)};
  const json midPerpendicularLines{filterDependenciesByType(dependencies, DependencyType::MID_PERPENDICULAR_LINE)};
  const json bisectors{filterDependenciesByType(dependencies, DependencyType::BISECTOR_LINE)};
  const json tangentLineCircles{filterDependenciesByType(dependencies, DependencyType::TANGENT_LINE_TO_CIRCLE)};
  const json tangentCircleCircles{filterDependenciesByType(dependencies, DependencyType::TANGENT_CIRCLE_TO_CIRCLE)};

  const std::vector<json> expectedParallelLines{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINES}, {"id",663}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L24"}}}, {"object2",{{"id","jxgBoard1L28"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::PARALLEL_LINES}}
  };
  
  const std::vector<json> expectedPerpendicularLines{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINES}, {"id",662}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L24"}}}, {"object2",{{"id","jxgBoard1L31"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::PERPENDICULAR_LINES}}
  };

  const std::vector<json> expectedMidPerpendicualarLines{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINE_AND_POINTS_PAIR}, {"id",669}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L40"}}}, {"object2",{{"end1Id","jxgBoard1P20"}, {"end2Id","jxgBoard1P22"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::MID_PERPENDICULAR_LINE}}
  };

  const std::vector<json> expectedBisectors{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINE_AND_ANGLE}, {"id",667}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L63"}}}, {"object2",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P45"}, {"vertexId","jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::BISECTOR_LINE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINE_AND_ANGLE}, {"id",668}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L82"}}}, {"object2",{{"point1Id","jxgBoard1P47"}, {"point2Id","jxgBoard1P51"}, {"vertexId","jxgBoard1P49"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::BISECTOR_LINE}}
  };

  const std::vector<json> expectedLineCircles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINE_AND_CIRCLE}, {"id",664}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L90"}}}, {"object2",{{"id","jxgBoard1C87"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::TANGENT_LINE_TO_CIRCLE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_LINE_AND_CIRCLE}, {"id",665}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1L90"}}}, {"object2",{{"id","jxgBoard1C95"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::TANGENT_LINE_TO_CIRCLE}}
  };

  const std::vector<json> expectedCircleCircles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_CIRCLES}, {"id",666}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1C100"}}}, {"object2",{{"id","jxgBoard1C95"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::TANGENT_CIRCLE_TO_CIRCLE}}
  };
 
  REQUIRE(parallelLines == json(expectedParallelLines));
  REQUIRE(perpendicularLines == json(expectedPerpendicularLines));
  REQUIRE(midPerpendicularLines == json(expectedMidPerpendicualarLines));
  REQUIRE(bisectors == json(expectedBisectors));
  REQUIRE(tangentLineCircles == json(expectedLineCircles));
  REQUIRE(tangentCircleCircles == json(expectedCircleCircles));
}

TEST_CASE("Importing task(part 3) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Divide Segment
  //  -> Divide Angle
  //  -> Equal Segments
  //  -> Equal Angles
  const json task4 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-171.89710807186995,\"y\":19.292604501607716,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-105.27330012183478,\"y\":75.11253724910823,\"name\":\"B\"},{\"id\":\"jxgBoard1P35\",\"x\":-51.768488745980676,\"y\":65.08038388954482,\"name\":\"E\"},{\"id\":\"jxgBoard1P37\",\"x\":-37.36334797653734,\"y\":25.209001252888868,\"name\":\"F\"},{\"id\":\"jxgBoard1P39\",\"x\":15.369770994523751,\"y\":48.87459807073955,\"name\":\"G\"},{\"id\":\"jxgBoard1P41\",\"x\":78.90676026175647,\"y\":41.157556270096464,\"name\":\"H\"},{\"id\":\"jxgBoard1P43\",\"x\":96.14147909967849,\"y\":19.292604501607716,\"name\":\"I\"},{\"id\":\"jxgBoard1P45\",\"x\":136.78457376657963,\"y\":37.89924875077461,\"name\":\"J\"},{\"id\":\"jxgBoard1P47\",\"x\":-30.950386239946532,\"y\":67.1149573416256,\"name\":\"K\"},{\"id\":\"jxgBoard1P49\",\"x\":-11.693504865065652,\"y\":58.94761990538467,\"name\":\"L\"},{\"id\":\"jxgBoard1P63\",\"x\":68.94329380728561,\"y\":13.34530544563057,\"name\":\"M\"},{\"id\":\"jxgBoard1P65\",\"x\":89.60555393635084,\"y\":-7.770162014769422,\"name\":\"N\"},{\"id\":\"jxgBoard1P67\",\"x\":117.6274027611557,\"y\":1.5876127288409947,\"name\":\"O\"},{\"id\":\"jxgBoard1P101\",\"x\":-161.09324758842442,\"y\":-30.868167202572348,\"name\":\"P\"},{\"id\":\"jxgBoard1P103\",\"x\":-122.50803858520896,\"y\":-66.62379813730908,\"name\":\"Q\"},{\"id\":\"jxgBoard1P108\",\"x\":-88.29581208551042,\"y\":-76.14147517458802,\"name\":\"S\"},{\"id\":\"jxgBoard1P110\",\"x\":-37.36334797653734,\"y\":-70.7395498392283,\"name\":\"T\"},{\"id\":\"jxgBoard1P117\",\"x\":-63.086820645347636,\"y\":-11.061091285043208,\"name\":\"V\"},{\"id\":\"jxgBoard1P119\",\"x\":-44.565908548532924,\"y\":-38.842439804813104,\"name\":\"X\"},{\"id\":\"jxgBoard1P128\",\"x\":-4.437295110279305,\"y\":-17.49195945224578,\"name\":\"B'\"},{\"id\":\"jxgBoard1P139\",\"x\":31.061105022859763,\"y\":-41.92926830034164,\"name\":\"G'\"},{\"id\":\"jxgBoard1P141\",\"x\":49.06753589102697,\"y\":-70.7395498392283,\"name\":\"H'\"},{\"id\":\"jxgBoard1P154\",\"x\":96.14147909967849,\"y\":-52.21865736786576,\"name\":\"N'\"},{\"id\":\"jxgBoard1P25\",\"x\":-149.68917208852483,\"y\":37.89924875077461,\"name\":\"C\"},{\"id\":\"jxgBoard1P27\",\"x\":-127.48123610517982,\"y\":56.50589299994141,\"name\":\"D\"}],\"lines\":[{\"id\":\"jxgBoard1L24\",\"a\":0.8378376208901618,\"b\":163.31446856644223,\"type\":2,\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P25\",\"jxgBoard1P27\"]},{\"id\":\"jxgBoard1L105\",\"a\":-0.9266667683919263,\"b\":-180.14792635509812,\"type\":2,\"pointsOn\":[\"jxgBoard1P101\",\"jxgBoard1P103\"]},{\"id\":\"jxgBoard1L112\",\"a\":0.10606055351655432,\"b\":-66.77677247160511,\"type\":2,\"pointsOn\":[\"jxgBoard1P108\",\"jxgBoard1P110\"]},{\"id\":\"jxgBoard1L121\",\"a\":-1.4999989403625442,\"b\":-105.69125540390654,\"type\":2,\"pointsOn\":[\"jxgBoard1P117\",\"jxgBoard1P119\"]},{\"id\":\"jxgBoard1L130\",\"a\":0.5320512851863064,\"b\":-15.131090886070762,\"type\":2,\"pointsOn\":[\"jxgBoard1P119\",\"jxgBoard1P128\"]},{\"id\":\"jxgBoard1L143\",\"a\":-1.5999995640345983,\"b\":7.768486194666858,\"type\":2,\"pointsOn\":[\"jxgBoard1P139\",\"jxgBoard1P141\"]},{\"id\":\"jxgBoard1L156\",\"a\":0.3934425546054248,\"b\":-90.04480650838732,\"type\":2,\"pointsOn\":[\"jxgBoard1P141\",\"jxgBoard1P154\"]}],\"circles\":[],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[{\"segment1End1Id\":\"jxgBoard1P20\",\"segment1End2Id\":\"jxgBoard1P25\",\"segment2End1Id\":\"jxgBoard1P25\",\"segment2End2Id\":\"jxgBoard1P27\"},{\"segment1End1Id\":\"jxgBoard1P20\",\"segment1End2Id\":\"jxgBoard1P25\",\"segment2End1Id\":\"jxgBoard1P27\",\"segment2End2Id\":\"jxgBoard1P22\"},{\"segment1End1Id\":\"jxgBoard1P25\",\"segment1End2Id\":\"jxgBoard1P27\",\"segment2End1Id\":\"jxgBoard1P27\",\"segment2End2Id\":\"jxgBoard1P22\"},{\"segment1End1Id\":\"jxgBoard1P101\",\"segment1End2Id\":\"jxgBoard1P103\",\"segment2End1Id\":\"jxgBoard1P108\",\"segment2End2Id\":\"jxgBoard1P110\"}],\"equalAngles\":[{\"angle1End1Id\":\"jxgBoard1P35\",\"angle1VertexId\":\"jxgBoard1P37\",\"angle1End2Id\":\"jxgBoard1P47\",\"angle2End1Id\":\"jxgBoard1P47\",\"angle2VertexId\":\"jxgBoard1P37\",\"angle2End2Id\":\"jxgBoard1P49\"},{\"angle1End1Id\":\"jxgBoard1P35\",\"angle1VertexId\":\"jxgBoard1P37\",\"angle1End2Id\":\"jxgBoard1P47\",\"angle2End1Id\":\"jxgBoard1P49\",\"angle2VertexId\":\"jxgBoard1P37\",\"angle2End2Id\":\"jxgBoard1P39\"},{\"angle1End1Id\":\"jxgBoard1P47\",\"angle1VertexId\":\"jxgBoard1P37\",\"angle1End2Id\":\"jxgBoard1P49\",\"angle2End1Id\":\"jxgBoard1P49\",\"angle2VertexId\":\"jxgBoard1P37\",\"angle2End2Id\":\"jxgBoard1P39\"},{\"angle1End1Id\":\"jxgBoard1P41\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P63\",\"angle2End1Id\":\"jxgBoard1P63\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P65\"},{\"angle1End1Id\":\"jxgBoard1P41\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P63\",\"angle2End1Id\":\"jxgBoard1P65\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P67\"},{\"angle1End1Id\":\"jxgBoard1P41\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P63\",\"angle2End1Id\":\"jxgBoard1P67\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P45\"},{\"angle1End1Id\":\"jxgBoard1P63\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P65\",\"angle2End1Id\":\"jxgBoard1P65\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P67\"},{\"angle1End1Id\":\"jxgBoard1P63\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P65\",\"angle2End1Id\":\"jxgBoard1P67\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P45\"},{\"angle1End1Id\":\"jxgBoard1P65\",\"angle1VertexId\":\"jxgBoard1P43\",\"angle1End2Id\":\"jxgBoard1P67\",\"angle2End1Id\":\"jxgBoard1P67\",\"angle2VertexId\":\"jxgBoard1P43\",\"angle2End2Id\":\"jxgBoard1P45\"},{\"angle1End1Id\":\"jxgBoard1P117\",\"angle1VertexId\":\"jxgBoard1P119\",\"angle1End2Id\":\"jxgBoard1P128\",\"angle2End1Id\":\"jxgBoard1P139\",\"angle2VertexId\":\"jxgBoard1P141\",\"angle2End2Id\":\"jxgBoard1P154\"}],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[],\"angleMeasures\":[],\"formulas\":[]}");

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task4));
  const json solution = expert.exportSolution();
  const json dependencies{solution["dependencies"]};

  const json equalSegments{filterDependenciesByType(dependencies, DependencyType::EQUAL_SEGMENTS)};
  const json equalAngles{filterDependenciesByType(dependencies, DependencyType::EQUAL_ANGLES)};

  const std::vector<json> expectedEqualSegments{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POINTS_PAIRS}, {"id",1263}, {"importance", DependencyImportance::HIGH}, {"object1",{{"end1Id","jxgBoard1P20"}, {"end2Id","jxgBoard1P25"}}}, {"object2",{{"end1Id","jxgBoard1P25"}, {"end2Id","jxgBoard1P27"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_SEGMENTS}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POINTS_PAIRS}, {"id",1264}, {"importance", DependencyImportance::HIGH}, {"object1",{{"end1Id","jxgBoard1P20"}, {"end2Id","jxgBoard1P25"}}}, {"object2",{{"end1Id","jxgBoard1P22"}, {"end2Id","jxgBoard1P27"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_SEGMENTS}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POINTS_PAIRS}, {"id",1265}, {"importance", DependencyImportance::HIGH}, {"object1",{{"end1Id","jxgBoard1P25"}, {"end2Id","jxgBoard1P27"}}}, {"object2",{{"end1Id","jxgBoard1P22"}, {"end2Id","jxgBoard1P27"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_SEGMENTS}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POINTS_PAIRS}, {"id",1266}, {"importance", DependencyImportance::HIGH}, {"object1",{{"end1Id","jxgBoard1P101"}, {"end2Id","jxgBoard1P103"}}}, {"object2",{{"end1Id","jxgBoard1P108"}, {"end2Id","jxgBoard1P110"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_SEGMENTS}},
  };

  const std::vector<json> expectedEqualAngles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1267}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P35"}, {"point2Id","jxgBoard1P47"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P47"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1268}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P35"}, {"point2Id","jxgBoard1P47"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P47"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1269}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P35"}, {"point2Id","jxgBoard1P47"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P39"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1270}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P35"}, {"point2Id","jxgBoard1P47"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P39"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1271}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P47"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P39"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1272}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P47"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P39"}, {"point2Id","jxgBoard1P49"}, {"vertexId", "jxgBoard1P37"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1273}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1274}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1275}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1276}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1277}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1278}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P41"}, {"point2Id","jxgBoard1P63"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1279}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1280}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1281}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1282}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P63"}, {"point2Id","jxgBoard1P65"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1283}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1284}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P65"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P45"}, {"point2Id","jxgBoard1P67"}, {"vertexId", "jxgBoard1P43"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1285}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P117"}, {"point2Id","jxgBoard1P128"}, {"vertexId", "jxgBoard1P119"}, {"type", AngleType::CONVEX}}}, {"object2",{{"point1Id","jxgBoard1P139"}, {"point2Id","jxgBoard1P154"}, {"vertexId", "jxgBoard1P141"}, {"type", AngleType::CONVEX}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_ANGLES}, {"id",1286}, {"importance", DependencyImportance::HIGH}, {"object1",{{"point1Id","jxgBoard1P117"}, {"point2Id","jxgBoard1P128"}, {"vertexId", "jxgBoard1P119"}, {"type", AngleType::CONCAVE}}}, {"object2",{{"point1Id","jxgBoard1P139"}, {"point2Id","jxgBoard1P154"}, {"vertexId", "jxgBoard1P141"}, {"type", AngleType::CONCAVE}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::EQUAL_ANGLES}},      
  };

  REQUIRE(equalSegments == json(expectedEqualSegments));
  REQUIRE(equalAngles == json(expectedEqualAngles));
}

TEST_CASE("Importing task(part 4) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Median
  //  -> Altitude
  //  -> Mid-Segment
  //  -> Polygon Perimeter
  //  -> Polygon Area
  const json task5 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-117.95121355754573,\"y\":47.056899652248475,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-73.72357128112296,\"y\":76.97559883924035,\"name\":\"B\"},{\"id\":\"jxgBoard1P25\",\"x\":-54.47154471544715,\"y\":27.804878048780488,\"name\":\"C\"},{\"id\":\"jxgBoard1P29\",\"x\":-32.61788220909553,\"y\":1.5284493299034552,\"name\":\"D\"},{\"id\":\"jxgBoard1P31\",\"x\":-0.09755700584349593,\"y\":61.62600633574695,\"name\":\"E\"},{\"id\":\"jxgBoard1P34\",\"x\":23.317077140497965,\"y\":9.073168746824187,\"name\":\"F\"},{\"id\":\"jxgBoard1P38\",\"x\":-113.52844734502033,\"y\":-22.666670636432926,\"name\":\"G\"},{\"id\":\"jxgBoard1P40\",\"x\":-54.47154471544715,\"y\":-22.666670636432926,\"name\":\"H\"},{\"id\":\"jxgBoard1P43\",\"x\":-112.7479615250254,\"y\":-68.71544318470528,\"name\":\"I\"},{\"id\":\"jxgBoard1P46\",\"x\":-33.91869521722561,\"y\":-69.2358001460874,\"name\":\"J\"},{\"id\":\"jxgBoard1P50\",\"x\":71.70731707317073,\"y\":-6.016260162601625,\"name\":\"K\"},{\"id\":\"jxgBoard1P52\",\"x\":42.56911363058943,\"y\":-26.308941104547763,\"name\":\"L\"},{\"id\":\"jxgBoard1P54\",\"x\":62.08130875254065,\"y\":-53.886182831554876,\"name\":\"M\"},{\"id\":\"jxgBoard1P56\",\"x\":97.72357723577235,\"y\":-57.00812611153455,\"name\":\"N\"},{\"id\":\"jxgBoard1P58\",\"x\":103.18700774898373,\"y\":-26.308941104547763,\"name\":\"O\"},{\"id\":\"jxgBoard1P67\",\"x\":7.282692777046908,\"y\":6.910390183078284,\"name\":\"Q\"},{\"id\":\"jxgBoard1P87\",\"x\":50.89430894308943,\"y\":24.422756288109756,\"name\":\"T\"},{\"id\":\"jxgBoard1P89\",\"x\":79.77235375381098,\"y\":76.97559883924035,\"name\":\"U\"},{\"id\":\"jxgBoard1P92\",\"x\":113.07319455030488,\"y\":24.682914919969512,\"name\":\"W\"},{\"id\":\"jxgBoard1P60\",\"x\":-86.2113791364964,\"y\":37.43088885051447,\"name\":\"P\"},{\"id\":\"jxgBoard1P74\",\"x\":-113.13820443502289,\"y\":-45.691056910567845,\"name\":\"R\"},{\"id\":\"jxgBoard1P80\",\"x\":-44.195119966336385,\"y\":-45.951235391260155,\"name\":\"S\"},{\"id\":\"jxgBoard1P96\",\"x\":65.33333134845022,\"y\":50.69917756367507,\"name\":\"V\"},{\"id\":\"jxgBoard1P102\",\"x\":96.42277415205788,\"y\":50.829256879605,\"name\":\"X\"}],\"lines\":[{\"id\":\"jxgBoard1L24\",\"a\":0.676470588235294,\"b\":126.84742647058822,\"type\":2,\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P22\"]},{\"id\":\"jxgBoard1L27\",\"a\":-2.554054276972883,\"b\":-111.31840370502695,\"type\":2,\"pointsOn\":[\"jxgBoard1P22\",\"jxgBoard1P25\"]},{\"id\":\"jxgBoard1L28\",\"a\":-0.3032785449992832,\"b\":11.284827223616285,\"type\":2,\"pointsOn\":[\"jxgBoard1P25\",\"jxgBoard1P20\",\"jxgBoard1P60\"]},{\"id\":\"jxgBoard1L33\",\"a\":1.8479998779296878,\"b\":61.806291670636924,\"type\":2,\"pointsOn\":[\"jxgBoard1P29\",\"jxgBoard1P31\"]},{\"id\":\"jxgBoard1L36\",\"a\":-2.2444441053602433,\"b\":61.40704508904493,\"type\":2,\"pointsOn\":[\"jxgBoard1P31\",\"jxgBoard1P34\"]},{\"id\":\"jxgBoard1L37\",\"a\":0.13488379190134447,\"b\":5.928072966057663,\"type\":2,\"pointsOn\":[\"jxgBoard1P34\",\"jxgBoard1P29\"]},{\"id\":\"jxgBoard1L42\",\"a\":0,\"b\":-22.666670636432926,\"type\":1,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P40\"]},{\"id\":\"jxgBoard1L45\",\"a\":-59.000139872588505,\"b\":-6720.860943510432,\"type\":2,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P43\",\"jxgBoard1P74\"]},{\"id\":\"jxgBoard1L48\",\"a\":-0.006601063104536763,\"b\":-69.45969959363984,\"type\":2,\"pointsOn\":[\"jxgBoard1P43\",\"jxgBoard1P46\"]},{\"id\":\"jxgBoard1L49\",\"a\":-2.265823506063436,\"b\":-146.08957706427864,\"type\":2,\"pointsOn\":[\"jxgBoard1P46\",\"jxgBoard1P40\",\"jxgBoard1P80\"]},{\"id\":\"jxgBoard1L66\",\"a\":3.1666654745740637,\"b\":310.43348667747244,\"type\":2,\"pointsOn\":[\"jxgBoard1P22\",\"jxgBoard1P60\"]},{\"id\":\"jxgBoard1L69\",\"a\":-7.4137892025708405,\"b\":60.9027392591893,\"type\":2,\"pointsOn\":[\"jxgBoard1P31\",\"jxgBoard1P67\"]},{\"id\":\"jxgBoard1L86\",\"a\":-0.003773815498644858,\"b\":-46.11801961995359,\"type\":2,\"pointsOn\":[\"jxgBoard1P74\",\"jxgBoard1P80\"]},{\"id\":\"jxgBoard1L91\",\"a\":1.8198199668842991,\"b\":-68.19572332730255,\"type\":2,\"pointsOn\":[\"jxgBoard1P87\",\"jxgBoard1P89\",\"jxgBoard1P96\"]},{\"id\":\"jxgBoard1L94\",\"a\":-1.5703112194325286,\"b\":202.24302093939033,\"type\":2,\"pointsOn\":[\"jxgBoard1P89\",\"jxgBoard1P92\",\"jxgBoard1P102\"]},{\"id\":\"jxgBoard1L95\",\"a\":0.004184034971343495,\"b\":24.209812719649513,\"type\":2,\"pointsOn\":[\"jxgBoard1P92\",\"jxgBoard1P87\"]},{\"id\":\"jxgBoard1L108\",\"a\":0.004184034971345046,\"b\":50.425820620518664,\"type\":2,\"pointsOn\":[\"jxgBoard1P96\",\"jxgBoard1P102\"]}],\"circles\":[],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[],\"medians\":[{\"polygonVerticesIds\":[\"jxgBoard1P22\",\"jxgBoard1P20\",\"jxgBoard1P25\"],\"segmentEnd1Id\":\"jxgBoard1P22\",\"segmentEnd2Id\":\"jxgBoard1P60\"}],\"altitudes\":[{\"polygonVerticesIds\":[\"jxgBoard1P31\",\"jxgBoard1P29\",\"jxgBoard1P34\"],\"segmentEnd1Id\":\"jxgBoard1P31\",\"segmentEnd2Id\":\"jxgBoard1P67\"}],\"midSegments\":[{\"polygonVerticesIds\":[\"jxgBoard1P38\",\"jxgBoard1P43\",\"jxgBoard1P40\",\"jxgBoard1P46\"],\"segmentEnd1Id\":\"jxgBoard1P74\",\"segmentEnd2Id\":\"jxgBoard1P80\"},{\"polygonVerticesIds\":[\"jxgBoard1P89\",\"jxgBoard1P87\",\"jxgBoard1P92\",\"jxgBoard1P89\"],\"segmentEnd1Id\":\"jxgBoard1P96\",\"segmentEnd2Id\":\"jxgBoard1P102\"}],\"segmentLengths\":[],\"angleMeasures\":[],\"perimeters\":[{\"polygonVerticesIds\":[\"jxgBoard1P50\",\"jxgBoard1P52\",\"jxgBoard1P54\",\"jxgBoard1P56\",\"jxgBoard1P58\"],\"perimeter\":\"32\"}],\"areas\":[{\"polygonVerticesIds\":[\"jxgBoard1P50\",\"jxgBoard1P52\",\"jxgBoard1P54\",\"jxgBoard1P56\",\"jxgBoard1P58\"],\"area\":\"214\"}],\"formulas\":[]}");

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task5));
  const json solution = expert.exportSolution();
  const json dependencies{solution["dependencies"]};

  const json medians{filterDependenciesByType(dependencies, DependencyType::MEDIAN)};
  const json altitudes{filterDependenciesByType(dependencies, DependencyType::ALTITUDE)};
  const json midSegments{filterDependenciesByType(dependencies, DependencyType::MID_SEGMENT)};
  const json perimeters{filterDependenciesByType(dependencies, DependencyType::POLYGON_PERIMETER)};
  const json areas{filterDependenciesByType(dependencies, DependencyType::POLYGON_AREA)};

  const std::vector<json> expectedMedians{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_POINTS_PAIRS}, {"id",1158}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P22", "jxgBoard1P20", "jxgBoard1P25"})}}}, {"object2",{{"end1Id","jxgBoard1P22"}, {"end2Id","jxgBoard1P60"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::MEDIAN}}
  };

  const std::vector<json> expectedAltitudes{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_POINTS_PAIRS}, {"id",1159}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P31", "jxgBoard1P29", "jxgBoard1P34"})}}}, {"object2",{{"end1Id","jxgBoard1P31"}, {"end2Id","jxgBoard1P67"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::ALTITUDE}}
  };

  const std::vector<json> expectedMidSegments{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_POINTS_PAIRS}, {"id",1160}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P38", "jxgBoard1P43", "jxgBoard1P40", "jxgBoard1P46"})}}}, {"object2",{{"end1Id","jxgBoard1P74"}, {"end2Id","jxgBoard1P80"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::MID_SEGMENT}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_POINTS_PAIRS}, {"id",1161}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P89", "jxgBoard1P87", "jxgBoard1P92", "jxgBoard1P89"})}}}, {"object2",{{"end1Id","jxgBoard1P102"}, {"end2Id","jxgBoard1P96"}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::MID_SEGMENT}}
  };

  const std::vector<json> expectedPerimeters{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_EXPRESSION}, {"id",1156}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P50", "jxgBoard1P52", "jxgBoard1P54", "jxgBoard1P56", "jxgBoard1P58"})}}}, {"object2",{{"value","32"}, {"variables",json(std::vector<std::string>{})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_PERIMETER}}
  };

  const std::vector<json> expectedAreas{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_POLYGON_AND_EXPRESSION}, {"id",1157}, {"importance", DependencyImportance::HIGH}, {"object1",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P50", "jxgBoard1P52", "jxgBoard1P54", "jxgBoard1P56", "jxgBoard1P58"})}}}, {"object2",{{"value","214"}, {"variables",json(std::vector<std::string>{})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_AREA}}
  };

  REQUIRE(medians == json(expectedMedians));
  REQUIRE(altitudes == json(expectedAltitudes));
  REQUIRE(midSegments == json(expectedMidSegments));
  REQUIRE(perimeters == json(expectedPerimeters));
  REQUIRE(areas == json(expectedAreas));
}

TEST_CASE("Importing task(part 5) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Scalene&Right/Equilateral/Isosceles&Acute/Isosceles&Right/Isosceles&Obtuse Triangle
  //  -> Square
  //  -> Rectangle
  //  -> Regular Polygon
  //  -> Parallelogram
  //  -> Kite
  //  -> Rhombus
  //  -> Scalene/Isosceles/Right Trapezoid
  const json task6 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-99.99999007558435,\"y\":88.42275728055131,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-139.02439024390242,\"y\":59.02437039507114,\"name\":\"B\"},{\"id\":\"jxgBoard1P25\",\"x\":-98.0476833563352,\"y\":62.05877975589213,\"name\":\"C\"},{\"id\":\"jxgBoard1P31\",\"x\":-106.5040650406504,\"y\":34.04877453315549,\"name\":\"D\"},{\"id\":\"jxgBoard1P33\",\"x\":-133.56097957952235,\"y\":12.715439214939023,\"name\":\"E\"},{\"id\":\"jxgBoard1P35\",\"x\":-101.55731197705914,\"y\":-0.049868464640375976,\"name\":\"F\"},{\"id\":\"jxgBoard1P37\",\"x\":-93.756103515625,\"y\":-39.83739837398374,\"name\":\"G\"},{\"id\":\"jxgBoard1P39\",\"x\":-86.99186991869918,\"y\":-17.46342852832825,\"name\":\"H\"},{\"id\":\"jxgBoard1P42\",\"x\":-131.80975564895883,\"y\":-16.12330171120079,\"name\":\"I\"},{\"id\":\"jxgBoard1P44\",\"x\":-41.72357326600609,\"y\":78.27641184737043,\"name\":\"J\"},{\"id\":\"jxgBoard1P46\",\"x\":-41.46341463414634,\"y\":48.878049772929366,\"name\":\"K\"},{\"id\":\"jxgBoard1P48\",\"x\":-12.325211191565032,\"y\":78.53657047923018,\"name\":\"L\"},{\"id\":\"jxgBoard1P54\",\"x\":11.349601435467479,\"y\":36.91056910569105,\"name\":\"M\"},{\"id\":\"jxgBoard1P56\",\"x\":-19.86991075965447,\"y\":10.634150311229675,\"name\":\"N\"},{\"id\":\"jxgBoard1P59\",\"x\":39.29993682576256,\"y\":7.180334025333725,\"name\":\"O\"},{\"id\":\"jxgBoard1P76\",\"x\":-59.67479674796748,\"y\":27.28453101181402,\"name\":\"P\"},{\"id\":\"jxgBoard1P78\",\"x\":-76.06503866552337,\"y\":0.4877949536331301,\"name\":\"Q\"},{\"id\":\"jxgBoard1P80\",\"x\":-49.268302607342484,\"y\":-15.902446963922767,\"name\":\"R\"},{\"id\":\"jxgBoard1P82\",\"x\":-32.87806068978659,\"y\":10.894289094258124,\"name\":\"S\"},{\"id\":\"jxgBoard1P84\",\"x\":-60.19511401168699,\"y\":-51.544715447154466,\"name\":\"T\"},{\"id\":\"jxgBoard1P86\",\"x\":-68.26016061674288,\"y\":-69.2357802972561,\"name\":\"U\"},{\"id\":\"jxgBoard1P89\",\"x\":-27.413324914525404,\"y\":-66.48935892146757,\"name\":\"W\"},{\"id\":\"jxgBoard1P91\",\"x\":-35.478371519581295,\"y\":-84.1804237715692,\"name\":\"V\"},{\"id\":\"jxgBoard1P93\",\"x\":63.90243902439024,\"y\":84.5203232183689,\"name\":\"X\"},{\"id\":\"jxgBoard1P95\",\"x\":67.02440215320122,\"y\":64.74793969131098,\"name\":\"Y\"},{\"id\":\"jxgBoard1P97\",\"x\":51.46199728980559,\"y\":52.15804854855335,\"name\":\"Z\"},{\"id\":\"jxgBoard1P99\",\"x\":32.77762929759898,\"y\":59.34054093285366,\"name\":\"A'\"},{\"id\":\"jxgBoard1P101\",\"x\":29.65566616878801,\"y\":79.1129244599116,\"name\":\"B'\"},{\"id\":\"jxgBoard1P103\",\"x\":45.21807103218366,\"y\":91.7028156026692,\"name\":\"C'\"},{\"id\":\"jxgBoard1P105\",\"x\":57.974694558586044,\"y\":4.65040253429878,\"name\":\"D'\"},{\"id\":\"jxgBoard1P107\",\"x\":115.41464208587398,\"y\":4.650422383130081,\"name\":\"E'\"},{\"id\":\"jxgBoard1P109\",\"x\":129.72358517530486,\"y\":-16.943091415777438,\"name\":\"F'\"},{\"id\":\"jxgBoard1P111\",\"x\":72.28363764801693,\"y\":-16.94311126460874,\"name\":\"G'\"},{\"id\":\"jxgBoard1P131\",\"x\":39.29993682576256,\"y\":-32.552857437754064,\"name\":\"H'\"},{\"id\":\"jxgBoard1P133\",\"x\":14.73172319613821,\"y\":-17.723577235772357,\"name\":\"I'\"},{\"id\":\"jxgBoard1P135\",\"x\":-23.2520325203252,\"y\":-32.552857437754064,\"name\":\"J'\"},{\"id\":\"jxgBoard1P137\",\"x\":14.731723196138221,\"y\":-47.382137639735774,\"name\":\"K'\"},{\"id\":\"jxgBoard1P143\",\"x\":100.84556021341463,\"y\":52.260166571392276,\"name\":\"L'\"},{\"id\":\"jxgBoard1P145\",\"x\":122.20685748953181,\"y\":26.504065040650406,\"name\":\"M'\"},{\"id\":\"jxgBoard1P148\",\"x\":89.7273457965043,\"y\":18.456512147812482,\"name\":\"N'\"},{\"id\":\"jxgBoard1P150\",\"x\":68.36604852038711,\"y\":44.21261367855435,\"name\":\"O'\"},{\"id\":\"jxgBoard1P156\",\"x\":63.90243902439024,\"y\":-41.65854849466463,\"name\":\"P'\"},{\"id\":\"jxgBoard1P158\",\"x\":100.84556021341463,\"y\":-41.398389862804876,\"name\":\"Q'\"},{\"id\":\"jxgBoard1P160\",\"x\":115.41464208587398,\"y\":-69.2357802972561,\"name\":\"R'\"},{\"id\":\"jxgBoard1P163\",\"x\":41.3658579616362,\"y\":-69.75724220270992,\"name\":\"S'\"},{\"id\":\"jxgBoard1P165\",\"x\":-133.56097957952235,\"y\":-51.544715447154466,\"name\":\"T'\"},{\"id\":\"jxgBoard1P167\",\"x\":-106.24390640879065,\"y\":-51.544715447154466,\"name\":\"U'\"},{\"id\":\"jxgBoard1P169\",\"x\":-99.9963403370693,\"y\":-80.68291888973577,\"name\":\"W'\"},{\"id\":\"jxgBoard1P171\",\"x\":-139.80854565124372,\"y\":-80.68291888973577,\"name\":\"V'\"},{\"id\":\"jxgBoard1P185\",\"x\":-6.08130478277439,\"y\":-69.75724220270992,\"name\":\"X'\"},{\"id\":\"jxgBoard1P187\",\"x\":14.731723196138221,\"y\":-69.75724220270992,\"name\":\"Y'\"},{\"id\":\"jxgBoard1P189\",\"x\":36.58538570248984,\"y\":-88.60318005967896,\"name\":\"Z'\"},{\"id\":\"jxgBoard1P192\",\"x\":-6.081304782774387,\"y\":-88.60318005967896,\"name\":\"A''\"}],\"lines\":[{\"id\":\"jxgBoard1L61\",\"a\":-13.504014130934637,\"b\":-1261.9785217934636,\"type\":2,\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P25\"]},{\"id\":\"jxgBoard1L62\",\"a\":0.0740520552114369,\"b\":69.31941221714894,\"type\":2,\"pointsOn\":[\"jxgBoard1P25\",\"jxgBoard1P22\"]},{\"id\":\"jxgBoard1L63\",\"a\":0.7533334723578207,\"b\":163.75609703993888,\"type\":2,\"pointsOn\":[\"jxgBoard1P22\",\"jxgBoard1P20\"]},{\"id\":\"jxgBoard1L64\",\"a\":-6.893136277362674,\"b\":-700.0982598851456,\"type\":2,\"pointsOn\":[\"jxgBoard1P31\",\"jxgBoard1P35\"]},{\"id\":\"jxgBoard1L65\",\"a\":-0.3988701494511491,\"b\":-40.55804867078693,\"type\":2,\"pointsOn\":[\"jxgBoard1P35\",\"jxgBoard1P33\"]},{\"id\":\"jxgBoard1L66\",\"a\":0.7884614961387199,\"b\":118.02312899996228,\"type\":2,\"pointsOn\":[\"jxgBoard1P33\",\"jxgBoard1P31\"]},{\"id\":\"jxgBoard1L67\",\"a\":3.3076873418185784,\"b\":270.2784784428814,\"type\":2,\"pointsOn\":[\"jxgBoard1P39\",\"jxgBoard1P37\"]},{\"id\":\"jxgBoard1L68\",\"a\":-0.02990160725548573,\"b\":-20.06462525705749,\"type\":2,\"pointsOn\":[\"jxgBoard1P42\",\"jxgBoard1P39\"]},{\"id\":\"jxgBoard1L69\",\"a\":-0.6231753152021414,\"b\":-98.26388773445795,\"type\":2,\"pointsOn\":[\"jxgBoard1P37\",\"jxgBoard1P42\"]},{\"id\":\"jxgBoard1L70\",\"a\":0.8416665395100911,\"b\":27.357989340682376,\"type\":2,\"pointsOn\":[\"jxgBoard1P56\",\"jxgBoard1P54\"]},{\"id\":\"jxgBoard1L71\",\"a\":-1.06368079900323,\"b\":48.9829222289373,\"type\":2,\"pointsOn\":[\"jxgBoard1P54\",\"jxgBoard1P59\"]},{\"id\":\"jxgBoard1L72\",\"a\":-0.058371221607594205,\"b\":9.474319346954763,\"type\":2,\"pointsOn\":[\"jxgBoard1P59\",\"jxgBoard1P56\"]},{\"id\":\"jxgBoard1L73\",\"a\":0.008849426073500012,\"b\":78.64564152451022,\"type\":2,\"pointsOn\":[\"jxgBoard1P48\",\"jxgBoard1P44\"]},{\"id\":\"jxgBoard1L74\",\"a\":-113.00167849241016,\"b\":-4636.557399912369,\"type\":2,\"pointsOn\":[\"jxgBoard1P44\",\"jxgBoard1P46\"]},{\"id\":\"jxgBoard1L75\",\"a\":1.0178568752443788,\"b\":91.0818714294036,\"type\":2,\"pointsOn\":[\"jxgBoard1P46\",\"jxgBoard1P48\"]},{\"id\":\"jxgBoard1L113\",\"a\":1.6349201063029093,\"b\":124.84805605460552,\"type\":2,\"pointsOn\":[\"jxgBoard1P76\",\"jxgBoard1P78\"]},{\"id\":\"jxgBoard1L114\",\"a\":-0.6116506832014734,\"b\":-46.03743791388072,\"type\":2,\"pointsOn\":[\"jxgBoard1P78\",\"jxgBoard1P80\"]},{\"id\":\"jxgBoard1L115\",\"a\":1.6349201063029093,\"b\":64.6472915722375,\"type\":2,\"pointsOn\":[\"jxgBoard1P80\",\"jxgBoard1P82\"]},{\"id\":\"jxgBoard1L116\",\"a\":-0.6116506832014734,\"b\":-9.215599188989346,\"type\":2,\"pointsOn\":[\"jxgBoard1P82\",\"jxgBoard1P76\"]},{\"id\":\"jxgBoard1L117\",\"a\":2.1935477519759523,\"b\":80.49614157311765,\"type\":2,\"pointsOn\":[\"jxgBoard1P84\",\"jxgBoard1P86\"]},{\"id\":\"jxgBoard1L118\",\"a\":-0.4558824849375619,\"b\":-100.35439194145395,\"type\":2,\"pointsOn\":[\"jxgBoard1P86\",\"jxgBoard1P91\"]},{\"id\":\"jxgBoard1L119\",\"a\":2.193547751975951,\"b\":-6.3569216810240174,\"type\":2,\"pointsOn\":[\"jxgBoard1P91\",\"jxgBoard1P89\"]},{\"id\":\"jxgBoard1L120\",\"a\":-0.4558824849375621,\"b\":-78.9866136039022,\"type\":2,\"pointsOn\":[\"jxgBoard1P89\",\"jxgBoard1P84\"]},{\"id\":\"jxgBoard1L121\",\"a\":-1.509092157537287,\"b\":92.1395594282806,\"type\":2,\"pointsOn\":[\"jxgBoard1P105\",\"jxgBoard1P111\"]},{\"id\":\"jxgBoard1L122\",\"a\":0,\"b\":-16.943136242794356,\"type\":1,\"pointsOn\":[\"jxgBoard1P111\",\"jxgBoard1P109\"]},{\"id\":\"jxgBoard1L123\",\"a\":-1.509092157537288,\"b\":178.82175361989553,\"type\":2,\"pointsOn\":[\"jxgBoard1P109\",\"jxgBoard1P107\"]},{\"id\":\"jxgBoard1L124\",\"a\":0,\"b\":-4.650382500681106,\"type\":1,\"pointsOn\":[\"jxgBoard1P107\",\"jxgBoard1P105\"]},{\"id\":\"jxgBoard1L125\",\"a\":-6.333317438822043,\"b\":266.9316721665816,\"type\":2,\"pointsOn\":[\"jxgBoard1P99\",\"jxgBoard1P101\"]},{\"id\":\"jxgBoard1L126\",\"a\":0.808993934630907,\"b\":55.121670401923105,\"type\":2,\"pointsOn\":[\"jxgBoard1P101\",\"jxgBoard1P103\"]},{\"id\":\"jxgBoard1L127\",\"a\":-0.3844118456292546,\"b\":109.08517774394565,\"type\":2,\"pointsOn\":[\"jxgBoard1P103\",\"jxgBoard1P93\"]},{\"id\":\"jxgBoard1L128\",\"a\":-6.333317438822012,\"b\":489.2347546748,\"type\":2,\"pointsOn\":[\"jxgBoard1P93\",\"jxgBoard1P95\"]},{\"id\":\"jxgBoard1L129\",\"a\":0.8089939346309085,\"b\":10.525604877108353,\"type\":2,\"pointsOn\":[\"jxgBoard1P95\",\"jxgBoard1P97\"]},{\"id\":\"jxgBoard1L130\",\"a\":-0.3844118456292545,\"b\":71.94064990649521,\"type\":2,\"pointsOn\":[\"jxgBoard1P97\",\"jxgBoard1P99\"]},{\"id\":\"jxgBoard1L139\",\"a\":-0.6035961924435794,\"b\":-8.831565206450575,\"type\":2,\"pointsOn\":[\"jxgBoard1P131\",\"jxgBoard1P133\"]},{\"id\":\"jxgBoard1L140\",\"a\":0.3904111092298913,\"b\":-23.475005629644393,\"type\":2,\"pointsOn\":[\"jxgBoard1P133\",\"jxgBoard1P135\"]},{\"id\":\"jxgBoard1L141\",\"a\":-0.3904111092298913,\"b\":-41.630709245863734,\"type\":2,\"pointsOn\":[\"jxgBoard1P135\",\"jxgBoard1P137\"]},{\"id\":\"jxgBoard1L142\",\"a\":0.6035961924435796,\"b\":-56.27414966905756,\"type\":2,\"pointsOn\":[\"jxgBoard1P137\",\"jxgBoard1P131\"]},{\"id\":\"jxgBoard1L152\",\"a\":0.2477732106596146,\"b\":-3.7755204041528625,\"type\":2,\"pointsOn\":[\"jxgBoard1P148\",\"jxgBoard1P145\"]},{\"id\":\"jxgBoard1L153\",\"a\":-1.2057367676605606,\"b\":173.85336637603325,\"type\":2,\"pointsOn\":[\"jxgBoard1P145\",\"jxgBoard1P143\"]},{\"id\":\"jxgBoard1L154\",\"a\":0.24777321065961472,\"b\":27.273338336547027,\"type\":2,\"pointsOn\":[\"jxgBoard1P143\",\"jxgBoard1P150\"]},{\"id\":\"jxgBoard1L155\",\"a\":-1.2057367676605604,\"b\":126.64407203925093,\"type\":2,\"pointsOn\":[\"jxgBoard1P150\",\"jxgBoard1P148\"]},{\"id\":\"jxgBoard1L177\",\"a\":0,\"b\":80.68291888973576,\"type\":1,\"pointsOn\":[\"jxgBoard1P169\",\"jxgBoard1P171\"]},{\"id\":\"jxgBoard1L178\",\"a\":4.663928817731251,\"b\":571.3741861381948,\"type\":2,\"pointsOn\":[\"jxgBoard1P171\",\"jxgBoard1P165\"]},{\"id\":\"jxgBoard1L179\",\"a\":0,\"b\":-51.54471544715446,\"type\":1,\"pointsOn\":[\"jxgBoard1P165\",\"jxgBoard1P167\"]},{\"id\":\"jxgBoard1L180\",\"a\":-4.6639288177312626,\"b\":-547.0587322554562,\"type\":2,\"pointsOn\":[\"jxgBoard1P167\",\"jxgBoard1P169\"]},{\"id\":\"jxgBoard1L181\",\"a\":0.007042140011089435,\"b\":-70.0485463661546,\"type\":2,\"pointsOn\":[\"jxgBoard1P160\",\"jxgBoard1P163\"]},{\"id\":\"jxgBoard1L182\",\"a\":0.0070421400110893834,\"b\":-42.108558417324474,\"type\":2,\"pointsOn\":[\"jxgBoard1P158\",\"jxgBoard1P156\"]},{\"id\":\"jxgBoard1L183\",\"a\":1.2468037467530375,\"b\":-121.33234889693192,\"type\":2,\"pointsOn\":[\"jxgBoard1P156\",\"jxgBoard1P163\"]},{\"id\":\"jxgBoard1L184\",\"a\":-1.9107168645317045,\"b\":151.28892275011395,\"type\":2,\"pointsOn\":[\"jxgBoard1P160\",\"jxgBoard1P158\"]},{\"id\":\"jxgBoard1L202\",\"a\":-0.8623697676072193,\"b\":-57.053049493602316,\"type\":2,\"pointsOn\":[\"jxgBoard1P189\",\"jxgBoard1P187\"]},{\"id\":\"jxgBoard1L203\",\"a\":0,\"b\":69.7572422027099,\"type\":1,\"pointsOn\":[\"jxgBoard1P187\",\"jxgBoard1P185\"]},{\"id\":\"jxgBoard1L204\",\"a\":0,\"b\":-6.0813047827744,\"type\":0,\"pointsOn\":[\"jxgBoard1P185\",\"jxgBoard1P192\"]},{\"id\":\"jxgBoard1L205\",\"a\":0,\"b\":-88.60318005967896,\"type\":1,\"pointsOn\":[\"jxgBoard1P192\",\"jxgBoard1P189\"]}],\"circles\":[],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[],\"inscribedCircles\":[],\"escribedCircles\":[],\"polygonTypes\":[{\"polygonVerticesIds\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P25\"],\"polygonType\":3},{\"polygonVerticesIds\":[\"jxgBoard1P31\",\"jxgBoard1P33\",\"jxgBoard1P35\"],\"polygonType\":2},{\"polygonVerticesIds\":[\"jxgBoard1P37\",\"jxgBoard1P39\",\"jxgBoard1P42\"],\"polygonType\":1},{\"polygonVerticesIds\":[\"jxgBoard1P44\",\"jxgBoard1P46\",\"jxgBoard1P48\"],\"polygonType\":4},{\"polygonVerticesIds\":[\"jxgBoard1P54\",\"jxgBoard1P56\",\"jxgBoard1P59\"],\"polygonType\":5},{\"polygonVerticesIds\":[\"jxgBoard1P76\",\"jxgBoard1P78\",\"jxgBoard1P80\",\"jxgBoard1P82\"],\"polygonType\":6},{\"polygonVerticesIds\":[\"jxgBoard1P84\",\"jxgBoard1P86\",\"jxgBoard1P89\",\"jxgBoard1P91\"],\"polygonType\":7},{\"polygonVerticesIds\":[\"jxgBoard1P93\",\"jxgBoard1P95\",\"jxgBoard1P97\",\"jxgBoard1P99\",\"jxgBoard1P101\",\"jxgBoard1P103\"],\"polygonType\":8},{\"polygonVerticesIds\":[\"jxgBoard1P105\",\"jxgBoard1P107\",\"jxgBoard1P109\",\"jxgBoard1P111\"],\"polygonType\":9},{\"polygonVerticesIds\":[\"jxgBoard1P131\",\"jxgBoard1P133\",\"jxgBoard1P135\",\"jxgBoard1P137\"],\"polygonType\":10},{\"polygonVerticesIds\":[\"jxgBoard1P143\",\"jxgBoard1P145\",\"jxgBoard1P148\",\"jxgBoard1P150\"],\"polygonType\":11},{\"polygonVerticesIds\":[\"jxgBoard1P156\",\"jxgBoard1P158\",\"jxgBoard1P160\",\"jxgBoard1P163\"],\"polygonType\":12},{\"polygonVerticesIds\":[\"jxgBoard1P165\",\"jxgBoard1P167\",\"jxgBoard1P169\",\"jxgBoard1P171\"],\"polygonType\":13},{\"polygonVerticesIds\":[\"jxgBoard1P185\",\"jxgBoard1P187\",\"jxgBoard1P189\",\"jxgBoard1P192\"],\"polygonType\":14}],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[],\"angleMeasures\":[],\"formulas\":[]}");

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task6));
  const json solution = expert.exportSolution();
  const json dependencies{solution["dependencies"]};

  const json polygonTypes = filterDependenciesByType(dependencies, DependencyType::POLYGON_TYPE);

  const std::vector<json> expectedPolygonTypes{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5671}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::SCALENE_RIGHT_TRIANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P20", "jxgBoard1P22", "jxgBoard1P25"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5672}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::EQUILATERAL_TRIANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P31", "jxgBoard1P33", "jxgBoard1P35"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5673}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::ISOSCELES_ACUTE_TRIANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P37", "jxgBoard1P39", "jxgBoard1P42"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5674}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::ISOSCELES_RIGHT_TRIANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P44", "jxgBoard1P46", "jxgBoard1P48"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5675}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::OBTUSE_ISOSCELES_TRIANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P54", "jxgBoard1P56", "jxgBoard1P59"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5676}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::SQUARE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P76", "jxgBoard1P78", "jxgBoard1P80", "jxgBoard1P82"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5677}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::RECTANGLE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P84", "jxgBoard1P86", "jxgBoard1P89", "jxgBoard1P91"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5678}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::REGULAR_POLYGON))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P93", "jxgBoard1P95", "jxgBoard1P97", "jxgBoard1P99", "jxgBoard1P101", "jxgBoard1P103"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5679}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::PARALLELOGRAM))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P105", "jxgBoard1P107", "jxgBoard1P109", "jxgBoard1P111"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5680}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::KITE))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P131", "jxgBoard1P133", "jxgBoard1P135", "jxgBoard1P137"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5681}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::RHOMBUS))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P143", "jxgBoard1P145", "jxgBoard1P148", "jxgBoard1P150"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5682}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::SCALENE_TRAPEZOID))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P156", "jxgBoard1P158", "jxgBoard1P160", "jxgBoard1P163"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5683}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::ISOSCELES_TRAPEZOID))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P165", "jxgBoard1P167", "jxgBoard1P169", "jxgBoard1P171"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}},
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::POLYGON_TYPE}, {"id",5684}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id",std::to_string(static_cast<unsigned int>(PolygonType::RIGHT_TRAPEZOID))}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P185", "jxgBoard1P187", "jxgBoard1P189", "jxgBoard1P192"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::POLYGON_TYPE}}
  };

  REQUIRE(polygonTypes == json(expectedPolygonTypes));
}

TEST_CASE("Importing task(part 6) and exporting empty solution from expert", "[expert]") {
  // Testing dependencies:
  //  -> Inscribed Circle
  //  -> Circumscribed Circle
  //  -> Escribed Circle
  const json task7 = json::parse("{\"points\":[{\"id\":\"jxgBoard1P20\",\"x\":-54.731693422891254,\"y\":37.17071781313516,\"name\":\"A\"},{\"id\":\"jxgBoard1P22\",\"x\":-84.91057109057418,\"y\":14.536589335619919,\"name\":\"B\"},{\"id\":\"jxgBoard1P24\",\"x\":-115.76269254025335,\"y\":36.24407724352589,\"name\":\"C\"},{\"id\":\"jxgBoard1P26\",\"x\":-104.65147455351183,\"y\":72.29417105850438,\"name\":\"D\"},{\"id\":\"jxgBoard1P28\",\"x\":-66.93224273161721,\"y\":72.86686642587746,\"name\":\"E\"},{\"id\":\"jxgBoard1P30\",\"x\":-105.69647434365989,\"y\":-31.772331920096544,\"name\":\"F\"},{\"id\":\"jxgBoard1P32\",\"x\":-63.056898659806905,\"y\":-23.447146532012194,\"name\":\"G\"},{\"id\":\"jxgBoard1P34\",\"x\":-54.731713271722555,\"y\":-66.08672221586518,\"name\":\"H\"},{\"id\":\"jxgBoard1P36\",\"x\":-97.37128895557554,\"y\":-74.41190760394953,\"name\":\"I\"},{\"id\":\"jxgBoard1P38\",\"x\":-12.065032710873982,\"y\":4.1300753461636175,\"name\":\"J\"},{\"id\":\"jxgBoard1P40\",\"x\":30.081300813008127,\"y\":27.284550860645325,\"name\":\"K\"},{\"id\":\"jxgBoard1P43\",\"x\":23.837394404217477,\"y\":-23.447146532012194,\"name\":\"L\"},{\"id\":\"jxgBoard1P47\",\"x\":73.96929080804954,\"y\":-2.4482133686265066,\"name\":\"M\"},{\"id\":\"jxgBoard1P51\",\"x\":45.22245306941379,\"y\":-39.873348186775246,\"name\":\"N\"},{\"id\":\"jxgBoard1P53\",\"x\":51.24650320929604,\"y\":38.912351422850854,\"name\":\"O\"},{\"id\":\"jxgBoard1P55\",\"x\":27.131379174054786,\"y\":3.316457471426975,\"name\":\"P\"},{\"id\":\"jxgBoard1P58\",\"x\":-85.39773486776956,\"y\":46.622484375332554,\"name\":\"Q\"},{\"id\":\"jxgBoard1P70\",\"x\":-80.2140938076909,\"y\":-48.92952706798043,\"name\":\"R\"},{\"id\":\"jxgBoard1P72\",\"x\":-76.05150111364867,\"y\":-70.24931490990727,\"name\":\"S\"},{\"id\":\"jxgBoard1P74\",\"x\":-58.89430596576481,\"y\":-44.766934373938334,\"name\":\"T\"},{\"id\":\"jxgBoard1P76\",\"x\":-84.376686501733,\"y\":-27.6097392260543,\"name\":\"U\"},{\"id\":\"jxgBoard1P78\",\"x\":-101.53388164961781,\"y\":-53.09211976202269,\"name\":\"W\"}],\"lines\":[{\"id\":\"jxgBoard1L42\",\"a\":0.5493829137322536,\"b\":10.758398171138516,\"type\":2,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P40\"]},{\"id\":\"jxgBoard1L45\",\"a\":-0.7681158098245626,\"b\":-5.137267025109189,\"type\":2,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P43\"]},{\"id\":\"jxgBoard1L46\",\"a\":8.12499324481122,\"b\":-217.12581504017996,\"type\":2,\"pointsOn\":[\"jxgBoard1P43\",\"jxgBoard1P40\",\"jxgBoard1P55\"]},{\"id\":\"jxgBoard1L61\",\"a\":3.244477235348574,\"b\":411.83349789303406,\"type\":2,\"pointsOn\":[\"jxgBoard1P26\",\"jxgBoard1P24\"]},{\"id\":\"jxgBoard1L62\",\"a\":-0.7035979014704582,\"b\":-45.20631029636622,\"type\":2,\"pointsOn\":[\"jxgBoard1P24\",\"jxgBoard1P22\"]},{\"id\":\"jxgBoard1L63\",\"a\":0.7499990134408815,\"b\":78.21943388425238,\"type\":2,\"pointsOn\":[\"jxgBoard1P22\",\"jxgBoard1P20\"]},{\"id\":\"jxgBoard1L64\",\"a\":-2.925782086484586,\"b\":-122.96229036652632,\"type\":2,\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P28\"]},{\"id\":\"jxgBoard1L65\",\"a\":0.015183113221321163,\"b\":73.88310624542855,\"type\":2,\"pointsOn\":[\"jxgBoard1P28\",\"jxgBoard1P26\"]},{\"id\":\"jxgBoard1L66\",\"a\":0.19524550267128907,\"b\":-11.135570656285667,\"type\":2,\"pointsOn\":[\"jxgBoard1P32\",\"jxgBoard1P30\",\"jxgBoard1P76\"]},{\"id\":\"jxgBoard1L67\",\"a\":-5.121756897435827,\"b\":-573.1239784243857,\"type\":2,\"pointsOn\":[\"jxgBoard1P30\",\"jxgBoard1P36\",\"jxgBoard1P78\"]},{\"id\":\"jxgBoard1L68\",\"a\":0.19524550267128907,\"b\":-55.40060134606684,\"type\":2,\"pointsOn\":[\"jxgBoard1P36\",\"jxgBoard1P34\",\"jxgBoard1P72\"]},{\"id\":\"jxgBoard1L69\",\"a\":-5.121756897435827,\"b\":-346.4092521737902,\"type\":2,\"pointsOn\":[\"jxgBoard1P34\",\"jxgBoard1P32\",\"jxgBoard1P74\"]},{\"id\":\"jxgBoard1L49\",\"a\":-0.7681158098245626,\"b\":-5.137267025109189,\"type\":2,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P43\",\"jxgBoard1P51\"]},{\"id\":\"jxgBoard1L50\",\"a\":0.5493829137322536,\"b\":10.758398171138516,\"type\":2,\"pointsOn\":[\"jxgBoard1P38\",\"jxgBoard1P40\",\"jxgBoard1P53\"]}],\"circles\":[{\"id\":\"jxgBoard1C57\",\"cx\":73.96929080804954,\"cy\":-2.4482133686265066,\"r\":47.1913275520838,\"centerId\":\"jxgBoard1P47\",\"pointsOn\":[\"jxgBoard1P55\",\"jxgBoard1P51\",\"jxgBoard1P53\"]},{\"id\":\"jxgBoard1C60\",\"cx\":-85.39773486776956,\"cy\":46.622484375332554,\"r\":32.089593157989185,\"centerId\":\"jxgBoard1P58\",\"pointsOn\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P24\",\"jxgBoard1P26\",\"jxgBoard1P28\"]},{\"id\":\"jxgBoard1C80\",\"cx\":-80.2140938076909,\"cy\":-48.92952706798043,\"r\":21.722350967638498,\"centerId\":\"jxgBoard1P70\",\"pointsOn\":[\"jxgBoard1P72\",\"jxgBoard1P74\",\"jxgBoard1P76\",\"jxgBoard1P78\"]}],\"perpendicularLines\":[],\"parallelLines\":[],\"equalSegments\":[],\"equalAngles\":[],\"midPerpendiculars\":[],\"bisectors\":[],\"tangentLines\":[],\"tangentCircles\":[],\"circumscribedCircles\":[{\"circleId\":\"jxgBoard1C60\",\"polygonVerticesIds\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P24\",\"jxgBoard1P26\",\"jxgBoard1P28\"]}],\"inscribedCircles\":[{\"circleId\":\"jxgBoard1C80\",\"polygonVerticesIds\":[\"jxgBoard1P36\",\"jxgBoard1P34\",\"jxgBoard1P32\",\"jxgBoard1P30\"]}],\"escribedCircles\":[{\"circleId\":\"jxgBoard1C57\",\"polygonVerticesIds\":[\"jxgBoard1P38\",\"jxgBoard1P43\",\"jxgBoard1P40\"]}],\"polygonTypes\":[{\"polygonVerticesIds\":[\"jxgBoard1P20\",\"jxgBoard1P22\",\"jxgBoard1P24\",\"jxgBoard1P26\",\"jxgBoard1P28\"],\"polygonType\":8},{\"polygonVerticesIds\":[\"jxgBoard1P30\",\"jxgBoard1P32\",\"jxgBoard1P34\",\"jxgBoard1P36\"],\"polygonType\":8}],\"medians\":[],\"altitudes\":[],\"midSegments\":[],\"segmentLengths\":[],\"angleMeasures\":[],\"formulas\":[]}");

  Expert expert{};
  REQUIRE_NOTHROW(expert.importTask(task7));
  const json solution = expert.exportSolution();
  const json dependencies{solution["dependencies"]};

  const json inscribedCircles = filterDependenciesByType(dependencies, DependencyType::INSCRIBED_CIRCLE);
  const json circumscribedCircles = filterDependenciesByType(dependencies, DependencyType::CIRCUMSCRIBED_CIRCLE);
  const json escribedCircles = filterDependenciesByType(dependencies, DependencyType::ESCRIBED_CIRCLE);

  const std::vector<json> expectedInscribedCircles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_CIRCLE_AND_POLYGON}, {"id",966}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1C80"}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P36", "jxgBoard1P34", "jxgBoard1P32", "jxgBoard1P30"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::INSCRIBED_CIRCLE}}
  };

  const std::vector<json> expectedCircumscribedCircles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_CIRCLE_AND_POLYGON}, {"id",967}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1C60"}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P20", "jxgBoard1P22", "jxgBoard1P24", "jxgBoard1P26", "jxgBoard1P28"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::CIRCUMSCRIBED_CIRCLE}}
  };

  const std::vector<json> expectedEscribedCircles{
    {{"basedOn", json(std::vector<size_t>{})}, {"category", DependencyCategory::OF_CIRCLE_AND_POLYGON}, {"id",968}, {"importance", DependencyImportance::HIGH}, {"object1",{{"id","jxgBoard1C57"}}}, {"object2",{{"verticesIds",json(std::vector<std::string>{"jxgBoard1P38", "jxgBoard1P43", "jxgBoard1P40"})}}}, {"reason", DependencyReason::USER_DEFINED}, {"type", DependencyType::ESCRIBED_CIRCLE}}
  };

  REQUIRE(inscribedCircles == json(expectedInscribedCircles));
  REQUIRE(circumscribedCircles == json(expectedCircumscribedCircles));
  REQUIRE(escribedCircles == json(expectedEscribedCircles));
}