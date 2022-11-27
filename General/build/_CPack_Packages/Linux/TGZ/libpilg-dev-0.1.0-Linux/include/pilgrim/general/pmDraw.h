/*=============================================================================
 * Product        :
 * File           : pmDraw.h
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef pm_DRAW_H
#define pm_DRAW_H
#include <probt/pl.h>
#include <graphviz/gvc.h>
#include <pugixml.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <vector>

#include <pilgrim/general/pmBayesianNetwork.h>

namespace PILGRIM {
 class pmDraw {
  private:
   pmBayesianNetwork *bn;
   bool dynamic_drawing;
   std::map<std::string, std::string> node_map; /* mapping between a variable name and a label n1, n2 (ie: XRay->n1) */
   std::map<std::string, std::string> node_map_reverse; /* reverse mapping (n1->XRay) */
   std::vector<std::vector<std::string>> edge_map; /* list of edges [(n1,n2),(n1,n3)] */
   void check_position(const std::map<std::string, std::vector<double>>& position); /* check if a position mapping is valid or not (ie a position is given for each node) */
   std::string random_string( size_t length ) ; /* function that generates a random string, used as id for a dynamic drawing */
   void fix_dynamic_svg(pugi::xml_document& svg_dom); /* function that takes a svg and modify it to place the percentage bar and the clickable buttons in it */

   /* when generating a dynamic svg, random id are placed in it. The following mapping are used to map the variables to these random id  */
   /* the main interest in random id is that we can place several images on a same webpage without any conflict :) */
   std::map<std::string, std::map<std::string, std::string>> mapping_to_random_id; /* mapping between a variable and its value to a id {'XRAY': {'Normal': 'id1', 'Abnormal': 'id2'}, ''} */
   std::map<std::string, std::map<std::string, std::string>> mapping_from_random_id; /* mapping from a random id to a variable and its value : {'id1': {'variable': 'XRay', 'value': 'Normal'}, 'id2': {'variable': 'XRay', 'value': 'Abnormal'}} */

   const std::string dot(const std::string& sink, const std::map<std::string, std::vector<double>>& position);
   const std::vector<uint8_t> draw(const std::string& format, const std::string& sink, const std::map<std::string, std::vector<double>>& position);


  public:
   /**
    * \brief Main constructor
    * \details It constructs a new pmDraw object which can be used to
    * generate an image representation of the Bayesian Network
    *
    * \param bn : the BayesianNetwork to consider for the drawing
    * \param dynamic_drawing : the drawing is used in a dynamic app where it is possible to perform a query by clicking on the nodes
    */
    pmDraw(pmBayesianNetwork *bn, bool dynamic_drawing = 0);

   /**
    * \brief Generates a dot string describing the graph (DOT is the syntax used by the Graphviz library)
    */
    const std::string dot();
   /**
    * \brief Generates a dot string describing the graph (DOT is the syntax used by the Graphviz library)
    * \param sink_node : the name of the node to place at the bottom
    */
    const std::string dot(const std::string& sink_node);
   /**
    * \brief Generates a dot string describing the graph (DOT is the syntax used by the Graphviz library)
    * \param position : a map describing the position x,y of each node (ie {"myvariable": [1.2, 2.4], "myvariable2": [1.2,0.3]})
    */
    const std::string dot(const std::map<std::string, std::vector<double>>& position);

   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    */
    const std::vector<uint8_t> draw(const std::string& format);
   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    * \param sink_node : the name of the node to place at the bottom
    */
    const std::vector<uint8_t> draw(const std::string& format, const std::string& sink_node);
   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    * \param position : a map describing the position x,y of each node (ie {"myvariable": [1.2, 2.4], "myvariable2": [1.2,0.3]})
    */
    const std::vector<uint8_t> draw(const std::string& format, const std::map<std::string, std::vector<double>>& position);

   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    * \param filename : the filename to write the image
    */
    const std::vector<uint8_t> draw_and_save(const std::string& format, const std::string& filename);
   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    * \param filename : the filename to write the image
    * \param sink_node : the name of the node to place at the bottom
    */
    const std::vector<uint8_t> draw_and_save(const std::string& format, const std::string& filename, const std::string& sink_node);
   /**
    * \brief Generate a byte array containing the image representation of the BayesianNetwork
    * \param format : the format of the image to create ("image/png", "image/svg+xml", "plain/text")
    * \param filename : the filename to write the image
    * \param position : a map describing the position x,y of each node (ie {"myvariable": [1.2, 2.4], "myvariable2": [1.2,0.3]})
    */
    const std::vector<uint8_t> draw_and_save(const std::string& format, const std::string& filename, const std::map<std::string, std::vector<double>>& position);

   /**
    * \brief Get the mapping between variables and values and the random identifiers used in the dynamic drawing
    * \details This function only works for dynamic drawing and the mapping looks like this: {'XRAY': {'Normal': 'id1', 'Abnormal': 'id2'}, ''}
    */
    const std::map<std::string, std::map<std::string, std::string>> get_mapping_to_random_id();
   /**
    * \brief Get the mapping between the random identifiers used in the dynamic drawing and the variables and values
    * \details This function only works for dynamic drawing and the mapping looks like this: {'id1': {'variable': 'XRay', 'value': 'Normal'}, 'id2': {'variable': 'XRay', 'value': 'Abnormal'}}
    */
    const std::map<std::string, std::map<std::string, std::string>> get_mapping_from_random_id();

   /**
    * \brief Get the default positition of the node
    */
    const std::map<std::string, std::vector<double>> get_default_position();
   /**
    * \brief Get the default positition of the node
    * \param sink_node : the name of the node to place at the bottom
    */
    const std::map<std::string, std::vector<double>> get_default_position(const std::string& sink_node);


 };
};

#endif
