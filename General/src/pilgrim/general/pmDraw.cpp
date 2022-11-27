#include "pilgrim/general/pmDraw.h"

PILGRIM::pmDraw::pmDraw(PILGRIM::pmBayesianNetwork *bn, bool dynamic_drawing) {
 /* keep the args */
 this->bn = bn;
 this->dynamic_drawing = dynamic_drawing;
 /* build a node_map where each node is associated to a unique and simple label: n1, n2 */
 std::vector<std::string> nodes = this->bn->getVariables().get_names();
 for (size_t i = 0; i < nodes.size(); i++) {
  std::ostringstream identifier;
  identifier << "n" << i;
  this->node_map[nodes[i]] = identifier.str();
  this->node_map_reverse[identifier.str()] = nodes[i];
 }
 /* build a list of edges: [(n1, n2)] means there is an edge between n1 and n2 */
 std::vector<plVariable> variables = this->bn->getVariables().as_variable_vector();
 for(size_t i =0; i < variables.size(); i++) {
  std::string variable_name = variables[i].get_names()[0];
  std::vector<plVariable> children = this->bn->getChildren(this->bn->getNode(variables[i])).as_variable_vector();
  for(size_t j =0; j < children.size(); j++) {
   std::vector<std::string> couple;
   std::string child_name = children[j].get_names()[0];
   couple.push_back(node_map[variable_name]);
   couple.push_back(node_map[child_name]);
   this->edge_map.push_back(couple);
  }
 }
}

void PILGRIM::pmDraw::check_position(const std::map<std::string, std::vector<double>>& position) {
 if (position.size() == 0) { return; }
 for (std::map<std::string, std::string>::iterator it=this->node_map.begin(); it!=this->node_map.end(); ++it) {
  if(position.count(it->first) <= 0) {
   std::ostringstream error;
   error << "No position found for " << it->first;
   throw std::runtime_error(error.str());
  }
 }
}

std::string PILGRIM::pmDraw::random_string( size_t length ) {
 auto randchar = []() -> char {
  const char charset[] =
  "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";
  const size_t max_index = (sizeof(charset) - 1);
  return charset[ rand() % max_index ];
 };
 std::string str(length,0);
 std::generate_n( str.begin(), length, randchar );
 return str;
}

void PILGRIM::pmDraw::fix_dynamic_svg(pugi::xml_document& svg_dom) {
 /* clear id mapping */
 this->mapping_to_random_id.clear();
 this->mapping_from_random_id.clear();
 /* browse each variable */
 std::map<std::string, std::string>::iterator node_it;
 for (node_it = this->node_map.begin(); node_it != this->node_map.end(); node_it++) {
  /* select the node of the variable */
  std::ostringstream querystring;
  querystring << "/svg/g/g[@id='" << node_it->second << "']";
  pugi::xpath_query queryxpath(querystring.str().c_str());
  pugi::xpath_node_set svg_nodes = queryxpath.evaluate_node_set(svg_dom);
  /* we classify the tags in vectors wether they are bar, percent, input */
  size_t i = 0;
  std::vector<pugi::xml_node> svg_attribute_label;
  std::vector<pugi::xml_node> svg_attribute_bar;
  std::vector<pugi::xml_node> svg_attribute_percent;
  std::vector<pugi::xml_node> svg_attribute_input;
  for (pugi::xml_node child:  svg_nodes[0].node().children()) {
   switch(i) {
    case 3:
     svg_attribute_label.push_back(child); break;
    case 4:
     svg_attribute_bar.push_back(child); break;
    case 5:
     svg_attribute_percent.push_back(child); break;
    case 6:
     svg_attribute_input.push_back(child); break;
   }
   if(i==6) {
    i=2;
   }
   i++;
  }
  for(size_t i = 0; i < svg_attribute_label.size(); i++) {
   std::ostringstream bar_id;
   std::ostringstream percent_id;
   std::ostringstream input_id;
   std::string prefix = this->random_string(10);
   bar_id << prefix << "_bar";
   percent_id << prefix << "_percent";
   input_id << prefix << "_input";
   /* setting the percentage */
   svg_attribute_percent.at(i).append_attribute("id") = percent_id.str().c_str();
   /* setting the input */
   double x = svg_attribute_input.at(i).attribute("x").as_double() - 10.0;
   double y = svg_attribute_input.at(i).attribute("y").as_double() - 14.0;
   pugi::xml_node input_container = svg_nodes[0].node().insert_child_after("foreignObject", svg_attribute_input.at(i));
   input_container.append_attribute("x") = x;
   input_container.append_attribute("y") = y;
   input_container.append_attribute("width") = 50;
   input_container.append_attribute("height") = 50;
   pugi::xml_node input_div = input_container.append_child("div");
   input_div.append_attribute("xmlns") = "http://www.w3.org/1999/xhtml";
   pugi::xml_node input = input_div.append_child("input");
   input.append_attribute("id") = input_id.str().c_str();
   input.append_attribute("type") = "checkbox";
   input.text().set(" ");
   /* setting the bar */
   x = svg_attribute_bar.at(i).attribute("x").as_double() - 3.0;
   y = svg_attribute_bar.at(i).attribute("y").as_double() - 8.0;
   pugi::xml_node bar_container = svg_nodes[0].node().insert_child_after("g", svg_attribute_bar.at(i));
   bar_container.append_attribute("width") = "50";
   bar_container.append_attribute("height") = "10";
   bar_container.append_attribute("x") = x;
   bar_container.append_attribute("y") = y;
   pugi::xml_node lmarker = bar_container.append_child("rect");
   lmarker.append_attribute("fill") = "silver";
   lmarker.append_attribute("width") = "1";
   lmarker.append_attribute("height") = "10";
   lmarker.append_attribute("x") = x;
   lmarker.append_attribute("y") = y;
   pugi::xml_node bar = bar_container.append_child("rect");
   bar.append_attribute("id") = bar_id.str().c_str();
   bar.append_attribute("fill") = "steelblue";
   bar.append_attribute("width") = "0";
   bar.append_attribute("height") = "10";
   bar.append_attribute("x") = x;
   bar.append_attribute("y") = y;
   pugi::xml_node rmarker = bar_container.append_child("rect");
   rmarker.append_attribute("fill") = "silver";
   rmarker.append_attribute("width") = "1";
   rmarker.append_attribute("height") = "10";
   rmarker.append_attribute("x") = x + 50.0;
   rmarker.append_attribute("y") = y;
   /* remove the graphviz generated bar */
   svg_nodes[0].node().remove_child(svg_attribute_bar.at(i));
   /* remove the graphviz generated input */
   svg_nodes[0].node().remove_child(svg_attribute_input.at(i));
    /* fill the dict */
    this->mapping_to_random_id[node_it->first][svg_attribute_label.at(i).text().as_string()] = prefix;
    this->mapping_from_random_id[prefix]["variable"] = node_it->first;
    this->mapping_from_random_id[prefix]["value"] = svg_attribute_label.at(i).text().as_string();
  }
 }
}

const std::string PILGRIM::pmDraw::dot(const std::string& sink_node, const std::map<std::string, std::vector<double>>& position) {
 /* if position speicified, check all the variables have their position */
 check_position(position);
 plVariablesConjunction variables = this->bn->getVariables();
 /* create the dot string */
 std::ostringstream dot_string;
 dot_string << "digraph G { overlap = false; \n";
 if(this->dynamic_drawing) {
  dot_string << "node[shape=\"box\",fontname=\"Helvetica\",fontsize=9]\n";
 }
 /* browse nodes */
 std::map<std::string, std::string>::iterator node_it;
 for (node_it = this->node_map.begin(); node_it != this->node_map.end(); node_it++) {
  /* prepare the label */
  std::ostringstream label;
  if (this->dynamic_drawing) {
   label << "<";
   label << "<TABLE border=\"0\">";
   label << "<TR><TD colspan=\"4\" align=\"left\"><B>" << node_it->first << "</B></TD></TR>";
   /* insert the values a variable can take */
   plVariable variable = variables.get_variable_with_name(node_it->first);
   plValues values = plValues(variable);
   do {
    std::string value = values.values()[0].to_string();
    value = value.substr(1, value.size() - 2);
    label << "<TR><TD align=\"left\">" << value << "</TD><TD>-------------</TD><TD align=\"right\"> 100.00 %</TD><TD>x</TD></TR>";
   } while(values.next());
   label << "</TABLE>";
   label << ">";
  } else {
   label << "\"" << node_it->first << "\"";
  }
  /* write the node */
  if(sink_node == node_it->first) {
   dot_string << " {rank=sink; " << node_it->second << "[label=" << label.str() << ",id=\"" << node_it->second << "\"];}\n";
  } else if(position.count(node_it->first) > 0) {
   dot_string << " " << node_it->second << "[label=" << label.str() << ",id=\"" << node_it->second << "\",pos=\"" << position.at(node_it->first).at(0) << "," << position.at(node_it->first).at(1) << "!\"];\n";
  } else {
   dot_string << " " << node_it->second << "[label=" << label.str() << ",id=\"" << node_it->second << "\"];\n";
  }
 }
 /* browse edges */
 std::vector<std::vector<std::string>>::iterator edge_it;
 for (edge_it = this->edge_map.begin(); edge_it != this->edge_map.end(); edge_it++) {
  dot_string << " " << (*edge_it)[0] << "->" << (*edge_it)[1] << ";\n";
 }
 dot_string << "}\n";
 return dot_string.str().c_str();
}


const std::vector<uint8_t> PILGRIM::pmDraw::draw(const std::string& format, const std::string& sink, const std::map<std::string, std::vector<double>>& position) {
 const std::string dotstring = this->dot(sink, position);
 /* create the graph and put it in an intermediate resfile */
 FILE *resfile = tmpfile();
 GVC_t *gvc = gvContext();
 Agraph_t *g = agmemread(dotstring.c_str());
 gvLayout(gvc, g, (position.size()>0)?"nop":"dot");
 if (format == "image/png") {
  gvRender(gvc, g, "png", resfile);
 }
 else if (format == "image/svg+xml") {
  gvRender(gvc, g, "svg", resfile);
 }
 else if (format == "text/plain") {
  gvRender(gvc, g, "plain", resfile);
 }
 else {
  throw std::runtime_error("Unknown format");
 }
 gvFreeLayout(gvc, g);
 agclose(g);
 gvFreeContext(gvc);

 rewind(resfile);
 fseek(resfile, 0, SEEK_END);
 size_t ressize = ftell(resfile);
 rewind(resfile);

 if(this->dynamic_drawing && format == "image/svg+xml") {
  /* parse the svg document to: create the bar graph, create the percentage and the input */
  char reschar[ressize];
  fread(reschar, sizeof(char), ressize, resfile);
  fclose(resfile);
  pugi::xml_document svg_dom;
  pugi::xml_parse_result parse = svg_dom.load_buffer(reschar, ressize, pugi::parse_default,  pugi::xml_encoding::encoding_utf8 );
  if (parse.status != pugi::xml_parse_status::status_ok) {
   throw std::runtime_error(parse.description());
  }
  /* edit the dom */
  this->fix_dynamic_svg(svg_dom);
  /* move the modified dom to a vector<uint8_t> */
  std::ostringstream resstring;
  svg_dom.save(resstring, PUGIXML_TEXT("\t"), pugi::format_default);
  std::string resstring_ = resstring.str();
  std::vector<uint8_t> resvector(resstring_.begin(), resstring_.end());
  return resvector;
 } else {
  /* move the content of resfile into the vector<uint8_t> */
  std::vector<uint8_t> resvector(ressize);
  fread(&(resvector[0]), sizeof(uint8_t), ressize, resfile);
  fclose(resfile);
  return resvector;
 }
}


const std::string PILGRIM::pmDraw::dot() {
 return this->dot("", std::map<std::string, std::vector<double>>());
}

const std::string PILGRIM::pmDraw::dot(const std::string& sink_node) {
 return this->dot(sink_node, std::map<std::string, std::vector<double>>());
}

const std::string PILGRIM::pmDraw::dot(const std::map<std::string, std::vector<double>>& position) {
 return this->dot("", position);
}

const std::vector<uint8_t> PILGRIM::pmDraw::draw(const std::string& format) {
 return this->draw(format, "", std::map<std::string, std::vector<double>>());
}
const std::vector<uint8_t> PILGRIM::pmDraw::draw(const std::string& format, const std::string& sink_node) {
 return this->draw(format, sink_node, std::map<std::string, std::vector<double>>());
}
const std::vector<uint8_t> PILGRIM::pmDraw::draw(const std::string& format, const std::map<std::string, std::vector<double>>& position) {
 return this->draw(format, "", position);
}

const std::vector<uint8_t> PILGRIM::pmDraw::draw_and_save(const std::string& format, const std::string& filename) {
 std::vector<uint8_t> img = this->draw(format, "", std::map<std::string, std::vector<double>>());
 std::ofstream file(filename);
 if(!file.is_open()) {
  std::ostringstream error;
  error << "Unable to open" << filename;
  throw std::runtime_error(error.str());
 }
 std::vector<uint8_t>::iterator it;
 for (it = img.begin(); it != img.end(); it++) {
  file << *it;
 }
 file.close();
}
const std::vector<uint8_t> PILGRIM::pmDraw::draw_and_save(const std::string& format, const std::string& filename, const std::string& sink_node) {
 std::vector<uint8_t> img = this->draw(format, sink_node, std::map<std::string, std::vector<double>>());
 std::ofstream file(filename);
 if(!file.is_open()) {
  std::ostringstream error;
  error << "Unable to open" << filename;
  throw std::runtime_error(error.str());
 }
 std::vector<uint8_t>::iterator it;
 for (it = img.begin(); it != img.end(); it++) {
  file << *it;
 }
 file.close();
}
const std::vector<uint8_t> PILGRIM::pmDraw::draw_and_save(const std::string& format, const std::string& filename, const std::map<std::string, std::vector<double>>& position) {
 std::vector<uint8_t> img = this->draw(format, "", position);
 std::ofstream file(filename);
 if(!file.is_open()) {
  std::ostringstream error;
  error << "Unable to open" << filename;
  throw std::runtime_error(error.str());
 }
 std::vector<uint8_t>::iterator it;
 for (it = img.begin(); it != img.end(); it++) {
  file << *it;
 }
 file.close();
}

const std::map<std::string, std::map<std::string, std::string>> PILGRIM::pmDraw::get_mapping_to_random_id() {
 if(!this->dynamic_drawing) {
  std::ostringstream error;
  error << "This works only for dynamic drawing";
  throw std::runtime_error(error.str());
 }
 return this->mapping_to_random_id;
}

const std::map<std::string, std::map<std::string, std::string>> PILGRIM::pmDraw::get_mapping_from_random_id() {
 if(!this->dynamic_drawing) {
  std::ostringstream error;
  error << "This works only for dynamic drawing";
  throw std::runtime_error(error.str());
 }
 return this->mapping_from_random_id;
}

const std::map<std::string, std::vector<double>> PILGRIM::pmDraw::get_default_position() {
 return this->get_default_position("");
}

const std::map<std::string, std::vector<double>> PILGRIM::pmDraw::get_default_position(const std::string& sink_node) {
 std::vector<uint8_t> graph = this->draw("text/plain", sink_node, std::map<std::string, std::vector<double>>());
 std::map<std::string, std::vector<double>> res;

 /* convert the vector into a string */
 std::ostringstream graphstring;
 for(std::vector<uint8_t>::iterator it = graph.begin(); it != graph.end(); it++) {
  graphstring << *it;
 }
 /* parse the string line by line */
 std::istringstream iss(graphstring.str());
 for (std::string line; std::getline(iss, line); ) {
  if (line.rfind("node", 0) != 0) { /* ignore the lines that do not start with "node" */
   continue;
  }
  std::istringstream iss2(line);
  size_t itoken = 0;
  bool endloop = false;
  std::string name;
  std::vector<double> coord;
  /* iterate over the columns ""node name x y width height"", and extract columns 3 and 4 */
  for (std::string token; std::getline(iss2, token, ' '); ) {
   switch(itoken) {
    case 1:
     name = token;
     break;
    case 2:
     coord.push_back(strtod(token.c_str(), NULL));
     break;
    case 3:
     coord.push_back(strtod(token.c_str(), NULL));
     endloop = true;
     break;
   }
   if(endloop){ break; }
   itoken++;
  }

  /* output */
  res[this->node_map_reverse[name]] = coord;
 }
 return res;
}

