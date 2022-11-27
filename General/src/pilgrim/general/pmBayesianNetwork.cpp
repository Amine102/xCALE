#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmPDAG.h"
#include <pilgrim/general/pmUtility.h>

#include <random>
#include <time.h>
#include <string>
#include <iostream>
#include <vector>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <chrono>

using namespace boost;
using namespace std;
using namespace PILGRIM;

pmBayesianNetwork::pmBayesianNetwork() {}

//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(const plVariablesConjunction &variables)
    : graph(variables.dim())
{
  this->initBayesianNetwork(variables);
}

//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(pmBayesianNetwork &bn,
                                     bool copyComputableObjects,
                                     bool copyLearnObjects)
    : graph(bn.variables.dim())
{
    this->initBayesianNetwork(bn.variables);
    this->copyStructure(bn);
    if (copyComputableObjects) {
        this->copyComputableObjects(bn);
    }


    if(copyLearnObjects) {
        std::vector<plLearnObject*> LO_lst = bn.getLearnObjectList();
        std::vector<plLearnObject*> copied_LO_lst;
        for (auto& LO: LO_lst) {
            if(LO != nullptr) {
                plLearnObject *copied_LO = LO->clone();
                copied_LO_lst.emplace_back(copied_LO);
            }

            // Even if a LO is null, the vector size should be equal to the number of variables
            else {
                copied_LO_lst.emplace_back(nullptr);
            }
        }

        this->setLearnObject(copied_LO_lst);
    }

}

//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(const plJointDistribution &jd)
    : graph(jd.get_variables().dim())
{
  plComputableObjectList compObjList = jd.get_computable_object_list();
  plComputableObject compObj;
  plVariablesConjunction known;
  const plVariablesConjunction allVars = jd.get_variables();
  unsigned int i, n;
  this->initBayesianNetwork(allVars);
  n = compObjList.size();
  for (i = 0; i < n; i++)
  {
    compObj = compObjList[i];
    known = compObj.get_left_variables();
    if (known.dim() > 1)
      this->copyMultiComputableObject(jd, known, &compObj);
    else
      this->copySingleComputableObject(known[0], &compObj);
  }
}
//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(plJointDistribution &jd, pmGraph &gr)
    : graph(jd.get_variables().dim())
{
  plComputableObjectList compObjList = jd.get_computable_object_list();
  plComputableObject compObj;
  plVariablesConjunction known;
  pmEdgeIterator ei, ei_end;
  pmEdge e_t;
  const plVariablesConjunction allVars = jd.get_variables();
  unsigned int i, n;
  this->initBayesianNetwork(allVars);
  n = compObjList.size();
  for (i = 0; i < n; i++)
  {
    compObj = compObjList[i];
    known = compObj.get_left_variables();
    if (known.dim() > 1)
      this->copyMultiComputableObject(jd, known, &compObj);
    else
      this->copySingleComputableObject(known[0], &compObj);
  }
  unsigned int u, j, k;
  bool exists;
  pmEdge e;
  const pmGraph &graph = gr;
  k = static_cast<unsigned int>(boost::num_vertices(graph));
  for (u = 0; u < k; u++)
  {
    for (j = 0; j < k; j++)
    {
      if (j == u)
        continue;
      boost::tie(e, exists) = edge(vertex(u, graph), vertex(j, graph), graph);
      if (exists)
      {
        this->add_arrow(this->getNode(this->getVariables()[u]), this->getNode(this->getVariables()[j]));
        /*cout<<"--------------------------------------------------------"<<endl;
		cout<<this->getVariables()[u].name()<<endl;
		cout<<this->getVariables()[j].name()<<endl;*/
      }
    }
  }
}
//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(plBayesianNetwork &plBN)
{
  plJointDistribution jd = plBN.get_joint_distribution();
  *this = pmBayesianNetwork(jd);
}
//=============================================================================
pmBayesianNetwork::pmBayesianNetwork(const std::string &file_name, std::string jd_label)
{
  plJointDistribution loaded_jd;
  plLearnDistributions loaded_pld;
  boost::filesystem::path file(file_name);

  std::vector<plLearnObject *> learnObjList;
  plComputableObjectList compObjList;
  plComputableObject compObj;
  plVariablesConjunction known;

  if (file.extension().string() == ".xdsl")
  {

    loaded_jd.load_from_genie(file_name);
  }
  else if (file.extension().string() == ".xml")
  {
    // Serializer creation
    plSerializer serial_load("serial_l");

    // Load bayesian network
    std::string jd_name = jd_label;
    std::string pld_name = "Learn Object";

    // Create and write in serialization file
    std::ofstream serialFile(file_name, std::ios::app);
    if (serialFile)
    {
      try
      {
        serial_load.load(file_name);
        serial_load.get_object(jd_name, loaded_jd);
        serial_load.get_object(pld_name, loaded_pld);
      }
      catch (const std::exception &e)
      {
        std::cerr << " Error while loading from file : " << e.what();
      }
    }
    else
    {
      std::cout << "ERROR: Unable to read serialization file." << std::endl;
      exit(0);
    }
  }
  else
    cerr << "Unknown file type" << endl;
  //  *this = pmBayesianNetwork(jd);

  compObjList = loaded_jd.get_computable_object_list();

  plLearnDistributions *loaded_pldbis = new plLearnDistributions(loaded_pld);
  learnObjList = loaded_pldbis->get_learn_objects();

  pmGraph tmpGraph(loaded_jd.get_variables().dim());
  this->graph = tmpGraph;

  const plVariablesConjunction allVars = loaded_jd.get_variables();
  unsigned int i, n;
  this->initBayesianNetwork(allVars);
  n = compObjList.size();
  for (i = 0; i < n; i++)
  {
    compObj = compObjList[i];
    known = compObj.get_left_variables();
    if (known.dim() > 1)
      this->copyMultiComputableObject(loaded_jd, known, &compObj);
    else
      this->copySingleComputableObject(known[0], &compObj);
  }
  this->setLearnObject(learnObjList);
}

//=============================================================================
pmBayesianNetwork::~pmBayesianNetwork()
{
  pmNodeIterator vi, vi_end;

  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
    this->setComputableObject(*vi, NULL);
}

//=============================================================================

std::pair<pmNodeIterator, pmNodeIterator> pmBayesianNetwork::vertices() const
{
  return boost::vertices(graph);
}
//=============================================================================

const plVariablesConjunction &pmBayesianNetwork::getVariables()
{
  return this->variables;
}

const plVariable pmBayesianNetwork::getVariable(const unsigned int n) const
{
  // vector<plVariable> variables;
  // this->variables.get_all_variables(variables);
  // return variables[n];
  return this->variables[n];
}

//=============================================================================
const plVariable pmBayesianNetwork::getVariable(const pmNode &node) const
{
  // return this->getVariable(this->getVariableIndex(node));
  return this->variables[this->getVariableIndex(node)];
}

//=============================================================================
unsigned int pmBayesianNetwork::getVariableIndex(const pmNode &node) const
{
  unsigned int idx;
  idx = this->vertex_id[node];

  return idx;
}

//=============================================================================
unsigned int pmBayesianNetwork::getVariableIndex(const plVariable &var) const
{
  unsigned int i, idx = this->variables.dim();
  plVariablesConjunction_const_iterator vi, vi_end;
  for (i = 0, vi = this->variables.begin(), vi_end = this->variables.end();
       vi != vi_end; ++vi, ++i)
    if (*vi == var)
    {
      idx = i;
      break;
    }
  return idx;
}

//=============================================================================
plVariablesConjunction pmBayesianNetwork::getParents(const pmNode &node) const
{
  plVariablesConjunction parents;
  pmInEdgeIterator ei, ei_end;
  pmNode parent;

  for (boost::tie(ei, ei_end) = in_edges(node, graph); ei != ei_end; ++ei)
  {
    parent = boost::source(*ei, graph);
    parents ^= this->getVariable(parent);
  }
  return parents;
}

//===========================================================================
plVariablesConjunction pmBayesianNetwork::getChildren(const pmNode &node)
{
  plVariablesConjunction children;
  pmOutEdgeIterator ei, ei_end;
  pmNode child;

  for (boost::tie(ei, ei_end) = out_edges(node, graph); ei != ei_end; ++ei)
  {
    child = boost::target(*ei, graph);
    children ^= this->getVariable(child);
  }
  return children;
}
//=============================================================================
plComputableObject *pmBayesianNetwork::getComputableObject(const pmNode &node) const
{
  return this->vertex_compObj[node];
}

//=============================================================================
plLearnObject *pmBayesianNetwork::getLearnObject(const pmNode &node) const
{
  return this->vertex_learnObj[node];
}

//=============================================================================

void pmBayesianNetwork::setComputableObject(const pmNode &node, plComputableObject *pCompObj)
{
  //plComputableObject* pOldCompObj = this->vertex_compObj[node];
  //if (pOldCompObj) delete pOldCompObj;
  this->vertex_compObj[node] = pCompObj;
  // something should be added to check for cycles
}

//=============================================================================
/**
 * Set the learn objects to user-defined values passed as parameter 'vect'
 */
void pmBayesianNetwork::setLearnObject(vector<plLearnObject *> &vect)
{
  pmNodeIterator vi, vi_end;
  boost::tie(vi, vi_end) = vertices();
  
  
  for (vector<plLearnObject *>::iterator it = vect.begin(); it != vect.end(); ++it)
  {
    plLearnObject *plLearn = NULL;
    plLearn = *it;
    this->vertex_learnObj[*vi] = plLearn;
    ++vi;
  }
  
  

  //for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi) {
  //  this->vertex_learnObj[* vi] =
  //}
}
//=============================================================================
/**
 * Set the learn objects with ML algorihm
 */
void pmBayesianNetwork::setLearnObjectML()
{
  vector<plLearnObject *> learn_objs(0);
  this->setLearnObjectList(learn_objs);
  pmNodeIterator vi, vi_end;
  boost::tie(vi, vi_end) = vertices();
  for (vector<plLearnObject *>::iterator it = learn_objs.begin(); it != learn_objs.end(); ++it)
  {
    plLearnObject *plLearn = NULL;
    plLearn = *it;
    this->vertex_learnObj[*vi] = plLearn;
    ++vi;
  }
}
//=============================================================================
void pmBayesianNetwork::setLearnObjectMLbis()
{
  vector<plLearnObject *> learn_objs(0);
  this->setLearnObjectListbis(learn_objs);
  pmNodeIterator vi, vi_end;
  boost::tie(vi, vi_end) = vertices();
  for (vector<plLearnObject *>::iterator it = learn_objs.begin(); it != learn_objs.end(); ++it)
  {
    plLearnObject *plLearn = NULL;
    plLearn = *it;
    this->vertex_learnObj[*vi] = plLearn;
    ++vi;
  }
}
//=============================================================================
/**
 * Set the learn objects to be used with EAP algorihm
 */
void pmBayesianNetwork::setLearnObjectEAP(pmScoreValueType priorWeight)
{
  vector<plLearnObject *> learn_objs(0);
  this->setBayesianLearnObjectList(learn_objs, priorWeight);
  this->setLearnObject(learn_objs);
}
//=============================================================================
plComputableObjectList pmBayesianNetwork::getComputableObjectList() const
{
  plComputableObjectList compObjList;
  pmNodeIterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    plComputableObject *pCompObj = NULL;
    pCompObj = this->getComputableObject(*vi);
    if (pCompObj)
    {
      compObjList *= (*pCompObj);
    }
    else
    {
      pCompObj = this->newComputableObject(*vi, false);
      compObjList *= (*pCompObj);
      delete pCompObj;
    }
  }
  return compObjList;
}

//=============================================================================
vector<plLearnObject *> pmBayesianNetwork::getLearnObjectList() const
{
  vector<plLearnObject*> learnObjectList(0);
  pmNodeIterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    learnObjectList.push_back(this->vertex_learnObj[*vi]);
    //cout << this->vertex_learnObj[*vi] << endl;
  }
  return learnObjectList;
}

//=============================================================================
void pmBayesianNetwork::setLearnObjectList(
    vector<plLearnObject *> &learnObjectList)
{
  pmNodeIterator vi, vi_end;
  pmNode node;

  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    node = *vi;

    if (in_degree(node, graph) == 0)
    {
      learnObjectList.push_back(new plLearnHistogram(this->getVariable(node)));
    }
    else
    {
      learnObjectList.push_back(
          new plCndLearnObject<plLearnHistogram>(this->getVariable(node),
                                                 this->getParents(node)));
    }
  }
}

//=============================================================================
void pmBayesianNetwork::setLearnObjectListbis(
    vector<plLearnObject *> &learnObjectList)
{
  pmNodeIterator vi, vi_end;
  pmNode node;

  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    node = *vi;

    double init_freq[] = {0, 0};
    if (in_degree(node, graph) == 0)
    {
      learnObjectList.push_back(new plLearnHistogram(this->getVariable(node), init_freq, 0.0));
    }
    else
    {
      learnObjectList.push_back(
          new plCndLearnObject<plLearnHistogram>(this->getVariable(node),
                                                 this->getParents(node), *(new plLearnHistogram(this->getVariable(node), init_freq, 0.0))));
    }
  }
}

//=============================================================================
void pmBayesianNetwork::
    setBayesianLearnObjectList(vector<plLearnObject *> &learnObjectList,
                               pmScoreValueType priorWeight)
{
  pmNodeIterator vi, vi_end;
  pmNode node;
  vector<plFloat> alpha;
  plBayesLearnProbTable *pObj;

  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    node = *vi;
    unsigned int nodeCard = this->getCardinality(getVariableIndex(node));
    bool hasParents = (in_degree(node, graph) != 0);
    unsigned int dim = nodeCard;

    alpha.resize(dim, priorWeight);

    if (!hasParents)
    {
      pObj = new plBayesLearnProbTable(this->getVariable(node), alpha);
      learnObjectList.push_back(pObj);
    }
    else
    {
      pObj = new plBayesLearnProbTable(this->getVariable(node), alpha);
      learnObjectList.push_back(
          new plCndLearnObject<plBayesLearnProbTable>(this->getVariable(node),
                                                      this->getParents(node),
                                                      *pObj));
    }
    // FIXME Potential memory leak
    // we must figure out:
    // - in the if case, when to delete pObj
    // - in the else case, whether we have to delete it
    //     (does plCndLearnObject handle it?)
    // delete pObj;
  }
}

//=============================================================================
void pmBayesianNetwork::
    freeLearnObjectList(vector<plLearnObject *> &learnObjectList)
{
  for (unsigned int i = 0; i < learnObjectList.size(); i++)
    delete learnObjectList[i];
}

//=============================================================================
plComputableObject *pmBayesianNetwork::newComputableObject(const pmNode &node,
                                                           bool random) const
{
  pmInEdgeIterator ei, ei_end;
  plComputableObject *pCompObj = NULL;
  unsigned int nodeIdx = this->getVariableIndex(node);
  unsigned int cardinality = this->getCardinality(nodeIdx);
  unsigned int i;
  vector<plProbValue> params;
  plProbValue p;
  plVariable var;

  var = this->getVariable(node);
  if (in_degree(node, graph) == 0)
  {
    params.clear();
    p = 1.0 / cardinality;
    params.resize(cardinality, p);
    if (random)
    {
      for (i = 0; i < cardinality; i++)
        params[i] = pmUtility::getRandomProbability();
    }
    pCompObj = new plProbTable(var, params);
  }
  else
  {
    plVariablesConjunction parents = this->getParents(node);
    plValues values(parents);
    plDistributionTable *pCondCompObj = new plDistributionTable(var, parents);

    params.clear();
    p = 1.0 / cardinality;
    params.resize(cardinality, p);

    values.reset();
    do
    {
      if (random)
      {
        for (i = 0; i < cardinality; i++)
          params[i] = pmUtility::getRandomProbability();
      }
      pCompObj = new plProbTable(var, params);
      pCondCompObj->push(values, *pCompObj);
    } while (values.next());
    pCompObj = pCondCompObj;
  }
  return pCompObj;
}

//=============================================================================
void pmBayesianNetwork::initComputableObjectList(bool random,
                                                 bool resetExistingObjects)
{
  pmNodeIterator vi, vi_end;
  pmNode node;
  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    node = *vi;
    if (resetExistingObjects || this->vertex_compObj[node] == NULL)
      this->setComputableObject(node, this->newComputableObject(node, random));
  }
}

//=============================================================================
plJointDistribution pmBayesianNetwork::getJointDistribution() const
{
  return plJointDistribution(this->variables, this->getComputableObjectList());
}

//=============================================================================
const vector<unsigned int> &pmBayesianNetwork::getCardinalities()
{
  return this->cardinalities;
}

//=============================================================================
unsigned int pmBayesianNetwork::getCardinality(const unsigned int n) const
{
  return this->cardinalities[n];
}

//=============================================================================
unsigned int pmBayesianNetwork::getMaxCardinality()
{
  return this->maxCardinality;
}

//=============================================================================
unsigned int pmBayesianNetwork::computeDim()
{
  unsigned int dim = 0;
  pmNodeIterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(); vi != vi_end; ++vi)
  {
    dim += this->computeNodeDim(*vi);
  }
  return dim;
}

//=============================================================================
unsigned int pmBayesianNetwork::computeParentDim(const pmNode &node)
{
  unsigned int qi = 1;
  pmInEdgeIterator ei, ei_end;
  pmNode parent;

  for (boost::tie(ei, ei_end) = in_edges(node, graph); ei != ei_end; ++ei)
  {
    parent = boost::source(*ei, graph);
    unsigned int parentIdx = this->getVariableIndex(parent);
    unsigned int rj = this->cardinalities[parentIdx];
    qi *= rj;
  }
  return qi;
}

//=============================================================================
unsigned int pmBayesianNetwork::computeNodeDim(const pmNode &node)
{
  unsigned int nodeIdx = this->getVariableIndex(node);
  unsigned int ri = this->cardinalities[nodeIdx];
  unsigned int qi = this->computeParentDim(node);
  return (ri - 1) * qi;
}

//=============================================================================
void pmBayesianNetwork::printIt()
{
  this->summary();
}

//=============================================================================
void pmBayesianNetwork::summary(bool displayLearnObject)
{
  unsigned int i, j, k, n;
  pmNode node;
  plVariable var;
  plVariablesConjunction parents;
  vector<string> nodeName, parentNames;
  plDistribution *pDistribution;
  plCndDistribution *pCndDistribution;
  vector<plProbValue> probValues;
  vector<vector<plProbValue>> allTable;

  n = (unsigned int)this->variables.dim();
  for (i = 0; i < n; i++)
  {
    plComputableObject *pCompObj;
    // parent labels
    node = vertex(i, graph);
    var = this->getVariable(i);
    parents = this->getParents(node);
    nodeName = var.get_names();
    parentNames = parents.get_names();
    cout << nodeName[0];
    if (parentNames.size() > 0)
      cout << "\t|";
    for (j = 0; j < parentNames.size(); j++)
      cout << "\t" << parentNames[j];
    cout << endl;

    // probability table
    probValues.clear();
    allTable.clear();
    if ((pCompObj = this->getComputableObject(node)) == NULL)
    {
      cout << "no probability" << endl;
      continue;
    }
    if (pCompObj->is_conditional())
    {
      plValues parentVal(parents);
      pCndDistribution = (plCndDistribution *)pCompObj;
      plDistribution kernel, compDistribution;
      parentVal.reset();
      do
      {
        pCndDistribution->instantiate(kernel, parentVal);
        kernel.compile(compDistribution);
        probValues.clear();
        compDistribution.tabulate(probValues);
        allTable.push_back(probValues);
      } while (parentVal.next());
    }
    else
    {
      pDistribution = (plDistribution *)pCompObj;
      pDistribution->tabulate(probValues);
      allTable.push_back(probValues);
    }
    plValues nodeVal(var);
    for (nodeVal.reset(), j = 0;
         j < this->cardinalities[i];
         j++, nodeVal.next())
    {
      cout << nodeVal[0] << " :";
      for (k = 0; k < allTable.size(); k++)
        cout << "\t" << allTable[k][j];
      cout << endl;
    }
  }
  print_graph();

  if (displayLearnObject) {
    std::cout << std::endl << "Displaying Learn Objects:" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::vector<plLearnObject*> lst_LO = this->getLearnObjectList();

    for (auto it_LO = lst_LO.begin(); it_LO != lst_LO.end(); it_LO++) {
      // If there's no LO, the vector is still created but with nullptr
      if (*it_LO != 0) {
        std::cout << **it_LO << std::endl;
        std::cout << "---------------------------" << std::endl;
      }
    }
  }
}

//=============================================================================
void pmBayesianNetwork::printEdges(std::ostream &argstream)
{
  unsigned int i, j, n;
  pmNode node;
  plVariable var;
  plVariablesConjunction parents;
  vector<string> nodeName, parentNames;

  n = (unsigned int)this->variables.dim();
  for (i = 0; i < n; i++)
  {
    // parent labels
    node = boost::vertex(i, graph);
    var = this->getVariable(i);
    parents = this->getParents(node);
    nodeName = var.get_names();
    parentNames = parents.get_names();

    for (j = 0; j < parentNames.size(); j++)
    {
      argstream << nodeName[0];
      argstream << "\t" << parentNames[j];
      argstream << endl;
    }
  }
}

//=============================================================================
void pmBayesianNetwork::resetComputableObject(const pmNode &node,
                                              plComputableObject *pCompObj)
{
  this->vertex_compObj[node] = pCompObj;
}

//=============================================================================
void pmBayesianNetwork::computeCardinalities()
{
  plVariable var;
  unsigned int i, n;

  n = this->num_vertices();
  this->cardinalities.resize(n);
  this->maxCardinality = 0;
  for (i = 0; i < n; i++)
  {
    var = this->getVariable(i);
    this->cardinalities[i] = (unsigned int)var.cardinality();
    if (this->cardinalities[i] > this->maxCardinality)
      this->maxCardinality = this->cardinalities[i];
  }
}

//=============================================================================
void pmBayesianNetwork::copyStructure(pmBayesianNetwork &bn)
{
  pmEdgeIterator ei, ei_end;
  pmEdge e;
  pmNode src, tgt;
  for (boost::tie(ei, ei_end) = boost::edges(bn.graph); ei != ei_end; ++ei)
  {
    e = *ei;
    src = boost::source(e, bn.graph);
    unsigned int i = bn.getVariableIndex(src);
    tgt = boost::target(e, bn.graph);
    unsigned int j = bn.getVariableIndex(tgt);
    add_edge(vertex(i, graph), vertex(j, graph), graph);
  }
}

//=============================================================================
void pmBayesianNetwork::copy_structure(pmBayesianNetwork &bn)
{
  pmEdgeIterator ei, ei_end;
  pmEdge e;
  pmNode src, tgt;
  for (boost::tie(ei, ei_end) = boost::edges(bn.graph); ei != ei_end; ++ei)
  {
    e = *ei;
    src = boost::source(e, bn.graph);
    unsigned int i = bn.getVariableIndex(src);
    tgt = boost::target(e, bn.graph);
    unsigned int j = bn.getVariableIndex(tgt);
    add_edge(vertex(i, graph), vertex(j, graph), graph);
  }
}

//=============================================================================
void pmBayesianNetwork::copyComputableObjects(pmBayesianNetwork &bn)
{
  pmNodeIterator vi, vi_end;
  pmNode node;
  for (boost::tie(vi, vi_end) = bn.vertices(); vi != vi_end; ++vi)
  {
    plComputableObject *pCompObj;
    if ((pCompObj = bn.getComputableObject(*vi)) != NULL)
    {
      plComputableObject *pNewCO;
      pNewCO = pmBayesianNetwork::copyComputableObject(*pCompObj);
      node = vertex(bn.getVariableIndex(*vi), graph);
      this->setComputableObject(node, pNewCO);
    }
  }
}

//=============================================================================
plComputableObject *
pmBayesianNetwork::copyComputableObject(const plComputableObject &compObj)
{
  plComputableObject *pNewCO;
  if (compObj.is_conditional())
    pNewCO = new plCndDistribution(compObj);
  else
    pNewCO = new plDistribution(compObj);
  /*
    pNewCO = new plComputableObject();
    (*pNewCO) = compObj;
  */
  return pNewCO;
}

//=============================================================================
void pmBayesianNetwork::copySingleComputableObject(const plVariable &var,
                                                   plComputableObject *pCompObj)
{
  unsigned int varIdx = this->getVariableIndex(var);
  pmNode node, parent;
  if (varIdx >= this->variables.dim())
  {
    cerr << "pmBayesianNetwork::copySingleComputableObject(): warning! "
            "getVariableIndex() returned invalid index for right variable!"
         << endl;
    return;
  }
  node = vertex(varIdx, graph);
  plVariablesConjunction parents = pCompObj->get_right_variables();
  plVariablesConjunction_const_iterator vi, vi_end;

  this->setComputableObject(node,
                            pmBayesianNetwork::copyComputableObject(*pCompObj));
  if (parents.dim() == 0)
    return;

  for (vi = parents.begin(), vi_end = parents.end(); vi != vi_end; ++vi)
  {
    unsigned int parentIdx = this->getVariableIndex(*vi);
    if (parentIdx >= this->variables.dim())
    {
      cerr << "pmBayesianNetwork::copySingleComputableObject(): warning! "
              "getVariableIndex() returned invalid index for left variables!"
           << endl;
      continue;
    }
    parent = vertex(parentIdx, graph);
    add_edge(parent, node, graph);
  }
}

//=============================================================================
void pmBayesianNetwork::
    copyMultiComputableObject(const plJointDistribution &jd,
                              const plVariablesConjunction &vars,
                              plComputableObject *pCompObj)
{
  plVariablesConjunction_const_iterator vi, vi_end;
  plVariablesConjunction parents = pCompObj->get_left_variables();
  plCndDistribution cndDistribution;
  plCndDistribution compiledCndDistribution;

  for (vi = vars.begin(), vi_end = vars.end(); vi != vi_end; ++vi)
  {
    jd.ask(cndDistribution, *vi, parents);
    cndDistribution.compile(compiledCndDistribution);
    this->copySingleComputableObject(*vi, &compiledCndDistribution);
  }
}

//=============================================================================
void pmBayesianNetwork::
    initBayesianNetwork(const plVariablesConjunction &variables)
{
  pmNodeIterator vi, vi_end;
  unsigned int i;

  this->vertex_id = get(vertex_index, graph);
  this->vertex_compObj = get(computable_object_t(), graph);
  this->vertex_learnObj = get(learn_object_t(), graph);
  //  this->vertex_compObj = NULL;
  for (i = 0, boost::tie(vi, vi_end) = vertices(); vi != vi_end; vi++, i++)
  {
    this->vertex_id[*vi] = i;
    this->vertex_compObj[*vi] = NULL;
    this->vertex_learnObj[*vi] = NULL;
  }
  this->variables = variables;
  this->computeCardinalities();
}

//============================================================
void pmBayesianNetwork::naiveBayes(int numVar)
{
  int j;
  int n = this->getVariables().dim();
  for (j = 0; j < n; j++)
  {
    if (numVar != j)
    {
      add_edge(vertex(numVar, graph), vertex(j, graph), graph);
    }
  }
}

//============================================================
pmNode pmBayesianNetwork::getNode(const plVariable &var)
{
  return vertex(this->getVariables().get_variable_position(var), graph);
}

double pmBayesianNetwork::getForgettingFactor()
{
  return this->forgettingFactor;
}

pmGraph pmBayesianNetwork::getCPDAG()
{
  pmGraph cpdag = convertDAGintoCPDAG(this->get_graph());
  return cpdag;
}

void pmBayesianNetwork::setForgettingFactor(double forgFactor)
{
  if (forgFactor > 1)
  {
    this->forgettingFactor = 1;
  }
  else if (forgFactor < 0)
  {
    this->forgettingFactor = 0;
  }
  else
    this->forgettingFactor = forgFactor;
}

void pmBayesianNetwork::save_as_genie(
    const std::string &file_name)
{
  plJointDistribution jd(this->getJointDistribution());
  jd.save_as_genie(file_name);
}

void pmBayesianNetwork::draw_graph_dot(
    const std::string &file_name)
{
  plJointDistribution jd(this->getJointDistribution());
  jd.draw_graph_dot(file_name);
}

/**
 * \brief Generates a dataset that corresponds to the Bayesian Network. The size of the dataset must be specified.
 * \param dataset_filename the name of the file where the dataset will be stored
 * \param nb_rows the size of the dataset that will be generated
 * Generates a dataset that corresponds to the Bayesian Network. The size of the dataset must be specified.
 * The dataset is generated using the well known algorithm "Forward Sampling"
 * Henrion, M. 1988. Propagating uncertainty in Bayesian networks by probabilistic logic sampling. In Lemmer, J., and Kanal, L., eds., Uncertainty in Artificial Intelligence 2 . Elsevier Science Publishers B.V. (North Holland). 149–163.
 *
 * Modification septembre_2020 : generates continuous variables 
 * \todo for instant, the generated dataset contains 0 and 1 and not labels ; modify so that the generated dataset will contain labels
 */

void pmBayesianNetwork::generate_csvData(const std::string &dataset_filename,
                                         unsigned int nb_rows)
{
  plJointDistribution jd(this->getJointDistribution());
  std::ofstream out(dataset_filename.c_str());
  out << "\"" << variables[0].name() << "\"";
  for (unsigned int j = 1; j < variables.dim(); ++j)
    // Writes the variable names in the CSV
    out << ";\"" << variables[j].name() << "\"";
  out << std::endl;
  std::vector<unsigned int> values(variables.dim());
  for (unsigned int i = 0; i < nb_rows; ++i)
  {
    // Generates the data and writes it in the .csv
    jd.draw(values);
    //out << variables[0].get_type().get_values_as_strings().at(values[0]);
    if (variables[0].get_var_type() == 1)
    {
      out << values[0] << endl;
    } // in case the variable is continuous
    else
    {
      out << variables[0].get_type().get_values_as_strings().at(values[0]);
    }
    for (unsigned int j = 1; j < variables.dim(); ++j)
    {
      if (variables[j].get_var_type() == 1)
      {
        out << ";" << values[j] << endl;
      }
      else
      {
        out << ";" << variables[j].get_type().get_values_as_strings().at(values[j]);
      }
    }
    out << std::endl;
  }
}

void pmBayesianNetwork::generate_csvIncompleteData(
    const std::string &dataset_filename,
    unsigned int nb_rows,
    std::vector<unsigned int> &indexes,
    std::vector<double> &probs_unk)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  std::map<unsigned int, double> prob_unk = map<unsigned int, double>();
  for (int i = 0; i < indexes.size(); i++)
  {
    // Initializes probabilities for incomplete data for each variable
    prob_unk[indexes[i]] = probs_unk[i];
  }
  plJointDistribution jd(this->getJointDistribution());
  std::ofstream out(dataset_filename.c_str());

  out << "\"" << variables[0].name() << "\"";
  for (unsigned int j = 1; j < variables.dim(); ++j)
    // Writes the variable names in the CSV
    out << ";\"" << variables[j].name() << "\"";
  out << std::endl;

  std::vector<unsigned int> values(variables.dim());
  for (unsigned int i = 0; i < nb_rows; ++i)
  {
    // Generates the data and writes it in the .csv
    jd.draw(values);
    if (prob_unk[0] <= dis(gen))
      out << variables[0].get_type().get_values_as_strings().at(values[0]);
    for (unsigned int j = 1; j < variables.dim(); ++j)
    {
      if (prob_unk[j] <= dis(gen))
        out << ";" << variables[j].get_type().get_values_as_strings().at(values[j]);
      else
        out << ";";
    }
    out << std::endl;
  }
}

/*
 * \brief Queries the bayesian network about P(target_variable | evidence)
 * \param target_variable
 * \param evidence
 */
plDistribution pmBayesianNetwork::query(plVariable &target_variable,
                                        plValues &evidence)
{
  // Conditional distribution representing P(target_variable | evidence)
  plCndDistribution CndP;

  this->getJointDistribution().ask(CndP,
                                   target_variable,
                                   evidence.get_variables());

  // Distribution representing P(target_variable, evidence)
  plDistribution P;
  CndP.instantiate(P, evidence);

  // Normalized distribution
  plDistribution PNorm;
  P.compile(PNorm);

  return PNorm;
}

/*
 * \brief Queries the bayesian network about P(target_variable)
 * \param target_variable
 */

plDistribution pmBayesianNetwork::query(plVariable &target_variable)
{
  plDistribution p;
  this->getJointDistribution().ask(p, target_variable);

  return p;
}

/*
 * \brief Infers the parameters from an other bayesian network
 * \param pFromBN original bayesian network
 */
void pmBayesianNetwork::infereParameters(pmBayesianNetwork *pFromBN)
{
  pmNodeIterator vi, vi_end;
  pmNode nodeFrom, parentFrom, nodeTo, parentTo;
  plComputableObject *pCompObj = NULL;
  plVariable var;
  plVariablesConjunction parents;
  plJointDistribution fromJD = pFromBN->getJointDistribution();
  for (boost::tie(vi, vi_end) = boost::vertices(this->graph);
       vi != vi_end;
       ++vi)
  {
    nodeTo = *vi;
    unsigned int varIdx = this->getVariableIndex(nodeTo);
    nodeFrom = vertex(varIdx, pFromBN->graph);
    var = this->getVariable(varIdx);
    if (in_degree(nodeTo, this->graph) == 0)
    {
      plDistribution kernel;
      bool message_state = plError::message_is_ignored(50);
      plError::ignore_this_message(50, true);
      fromJD.ask(kernel, var);
      plError::ignore_this_message(50, message_state);
      pCompObj = new plDistribution(kernel.compile());
    }
    else
    {
      parents = this->getParents(nodeTo);
      plValues *pParentVal = new plValues(parents);
      plCndDistribution cndDistribution;
      plDistribution kernel;
      plDistributionTable *pCndDistribution = new plDistributionTable(var,
                                                                      parents);
      plError::ignore_this_message(50, true);
      fromJD.ask(cndDistribution, var, parents);
      plError::ignore_this_message(50, false);

      pParentVal->reset();
      do
      {
        plDistribution *pDistribution;
        cndDistribution.instantiate(kernel, *pParentVal);
        pDistribution = new plDistribution(kernel.compile());
        pCndDistribution->push(*pParentVal, *pDistribution);
        delete pDistribution;
      } while (pParentVal->next());

      pCompObj = pCndDistribution;
      delete pParentVal;
    }
    this->setComputableObject(nodeTo, pCompObj);
  }
}

/*
        WARNING ------------WARNING------------------------WARNING
*/
/*
The incremental learning here is only working for simple distributions where
the distributions are either plProbTable plDistributionTable!!!!!
*/
/**
 * \brief Learns parameters from data (Incrementally or non-incrementally). If you want the learning to be incremental Learning, you have to set the forgettingFactor parameter to a value from 0 to 1
 * The incremental learning only works for simple distributions (plProbTable and plDistributionTable)
 * \param DataDscptr data descriptor
 */
void pmBayesianNetwork::learnParameters(pmCSVDataSet *DataDscptr)
{
  // lambda function to check if the variable has a parent, in order whether to generate probTable or Distribution table
  auto hasParent = [](string headSignature) -> bool {
    if (headSignature.find('|') != string::npos)
    {
      return true;
    }
    return false;
  };
  // The variables in the computable Object lists are not in the same order, so we use this function to get the same 2 variables from the 2 compObjects lists
  // in order to make the calculations
  auto getCompObjectWithSameSignature = [](string headSignature, plComputableObjectList compObject_oldList) -> plComputableObject {
    for (int i = 0; i < compObject_oldList.size(); i++)
    {
      if (compObject_oldList[i].get_head_string() == headSignature)
      {
        return compObject_oldList[i];
      }
    }
  };
  //lambda Function to multiply the vectors of probabilities
  auto MultiplyTwoVectors = [](vector<plProbValue> vector1, vector<plProbValue> vector2, float alpha) -> vector<plProbValue> {
    vector<plProbValue> result_vect;
    plProbValue alphaProb(alpha);

    if (vector1.size() != vector2.size())
    {
      cout << "The 2 vectors are not of equal size! CANNOT perform multiplication!" << endl;
      return result_vect;
    }
    for (int i = 0; i < vector1.size(); i++)
    {
      // cout << "value 1 is " << vector1[i] << " and value 2 is " << vector2[i] << endl;
      // cout << "value entered in the result vector is <<" << alphaProb * vector1[i] + (1 - alphaProb) * vector2[i] << endl;
      result_vect.push_back(alphaProb * vector1[i] + (1 - alphaProb) * vector2[i]);
    }
    return result_vect;
  };
  /**
   * Create the learn objects with ML.
   * If you want an other initialisation of the learn objects, use
   * setLearnObject() or setLearnObjectEAP(pmScoreValueType p)
   * before calling learnParameters.
   * This function can do the learning procedure Incrementally or non-incrementally.
   * If you want to perform the incremental learning, you have to set the forgetting factor to a value between 0 and 1.
   * By default: the forgetting factor is set to 0. 
   * The incremental learning only works for the simple distributions (probTable and plDistributionTable)
   * TODO : conditionnal when vertex_lo = null (need boost graph methods)
   * */

  pmNodeIterator vi, vi_end;
  boost::tie(vi, vi_end) = vertices();

  if (this->vertex_learnObj[*vi] == NULL)
  {
    this->setLearnObjectML();
  }
  plDataDescriptor /*<rowDataType>*/ &data_descriptor = *DataDscptr->get_data_descriptor();
  plEMLearner emLearner(this->getLearnObjectList());
  emLearner.run(data_descriptor);
  plJointDistribution jd = emLearner.get_joint_distribution();

  plComputableObjectList compObjList = jd.get_computable_object_list();
  plComputableObject compObj;
  plVariablesConjunction known;
  unsigned int i, n;

  n = compObjList.size();

  double forgettingFactor = this->getForgettingFactor();
  plComputableObjectList old_compObjList = this->getComputableObjectList();
  plComputableObject old_compObj;
  plVariablesConjunction oldKnown;

  for (i = 0; i < n; i++)
  {
    compObj = compObjList[i];
    known = compObj.get_left_variables();

    if (forgettingFactor != 0)
    {
      vector<plProbValue> vect;
      compObj.tabulate(vect);

      //Computable Object from previous computed parameters
      string headSignature = compObj.get_head_string();
      old_compObj = getCompObjectWithSameSignature(headSignature, old_compObjList);

      vector<plProbValue> old_vect;
      old_compObj.tabulate(old_vect);
      oldKnown = old_compObj.get_left_variables();

      vector<plProbValue> result_vect = MultiplyTwoVectors(old_vect, vect, forgettingFactor);

      //If the variable doesn't have a parent, calculate a probTable
      // Else get a DistributionTable
      if (!hasParent(headSignature))
      {
        //create a computableObject from a probTable
        compObj = plProbTable(known, result_vect);
      }
      else
      {
        compObj = plDistributionTable(known, compObj.get_right_variables(), result_vect);
      }
    }

    unsigned int varIdx = this->getVariableIndex(known[0]);
    if (varIdx >= this->variables.dim())
    {
      cerr << "pmBayesianNetwork::learnParameters(): warning! "
              "getVariableIndex() returned invalid index for right variable!"
           << endl;
      return;
    }
    pmNode node = vertex(varIdx, graph);
    this->setComputableObject(node,
                              pmBayesianNetwork::copyComputableObject(compObj));
  }
  
  vector<plLearnObject *> listLearnObjects = emLearner.get_learn_objects();

  /*  Why does this exists?
      Ok, so the plEMLearner objects take care of deallocating the memory when it's destroyed,
      including the plLearnObject. Since the scope of the plEMLearner object is limited to this
      function, it's destroyed at the end of the function, and the plLearnObject are too. BUT.
      get_learn_objects() returns a vector of POINTERS to the LOs, and these aren't destroyed.
      The problem is, after the end of this function, these pointers point to de-allocated 
      memory, thus segfaulting when they are being accessed. The solution is to deep-copy
      the vector and its contents (and the memory pointed by the pointers) before setting it.

      Yes, I spent a lot of time debugging this (including trying to deep copy by hand); 
      this is why I felt the need to write a huge wall of text about why it happened and how 
      I fixed it.
  */
  std::vector<plLearnObject*> copiedListLearnObjects;
  copiedListLearnObjects.reserve(listLearnObjects.size());
  std::copy(listLearnObjects.begin(), listLearnObjects.end(), copiedListLearnObjects.begin());

  this->setLearnObject(copiedListLearnObjects);
}

//====================================================================================
void pmBayesianNetwork::update(plValues &old_line, plValues &new_line){

   vector<plLearnObject *> listLearnObjects = this->getLearnObjectList();   
   plLearnObject* l0;
   int m = listLearnObjects.size();
  
   for(int i=0; i<m; i++){
        /*cout << "LearnObject i=" << i << " avant mise a jour \n" << endl;
        l0=listLearnObjects[i];
        cout << *l0 << "\n" << endl;*/
        
        //Mise a jour
	listLearnObjects[i]->add_point(old_line, weight=-PL_ONE);
	listLearnObjects[i]->add_point(new_line, weight=PL_ONE);
	
	/*cout << "LearnObject i=" << i << " apres mise a jour" "\n" << endl;
	l0=listLearnObjects[i];
	cout << *l0 << "\n" << endl;*/
   }

}

//====================================================================================
void pmBayesianNetwork::learnParametersbis(pmCSVDataSet *DataDscptr)
{
  /**
  * Create the learn objects with ML.
  * If you want an other initialisation of the learn objects, use
  * setLearnObject() or setLearnObjectEAP(pmScoreValueType p)
  * before calling learnParameters.
  * This function can do the learning procedure Incrementally or non-incrementally.
  * If you want to perform the incremental learning, you have to set the forgetting factor to a value between 0 and 1.
  * By default: the forgetting factor is set to 0. 
  * The incremental learning only works for the simple distributions (probTable and plDistributionTable)
  * TODO : conditionnal when vertex_lo = null (need boost graph methods)
  * */

  pmNodeIterator vi, vi_end;
  boost::tie(vi, vi_end) = vertices();
  if (this->vertex_learnObj[*vi] == NULL)
  {
    this->setLearnObjectMLbis();
  }
  plDataDescriptor /*<rowDataType>*/ &data_descriptor = *DataDscptr->get_data_descriptor();

  //getComputableobjectlist
  vector<plLearnObject *> listLearnObjects = this->getLearnObjectList();

  double forgettingFactor = this->getForgettingFactor();

  plLearnObject *lo;
  plComputableObject compObj;
  int m = listLearnObjects.size();

  for (int i = 0; i < m; ++i)
  {
    lo = listLearnObjects[i];
    lo->rescale_total_weight(forgettingFactor);
  }
  for (int j = 0; j < m; j++)
  {
    lo = listLearnObjects[j];
    data_descriptor.get_n_records();
    lo->learn(data_descriptor, data_descriptor.get_n_records(), 1);
    data_descriptor.rewind();
    compObj = lo->get_computable_object();
    plVariablesConjunction known = compObj.get_left_variables();
    unsigned int varIdx = this->getVariableIndex(known[0]);
    if (varIdx >= this->variables.dim())
    {
      cerr << "pmBayesianNetwork::learnParameters(): warning! "
              "getVariableIndex() returned invalid index for right variable!"
           << endl;
      return;
    }
    pmNode node = vertex(varIdx, graph);
    this->setComputableObject(node, pmBayesianNetwork::copyComputableObject(compObj));
  }
  this->setLearnObject(listLearnObjects);
}

//====================================================================================

void pmBayesianNetwork::noisyConditionals(pmCSVDataSet *DataDscptr, double epsilon)
{
  // lambda function to check if vect contains the plVariable elt
  auto contains = [](vector<plVariable> vect, plVariable elt) -> bool {
    bool isHere = false;
    for (vector<plVariable>::iterator it = vect.begin(); it != vect.end(); ++it)
    {
      if (*it == elt)
      {
        isHere = true;
      }
    }
    return isHere;
  };
  // Return the degree of the bayesian network
  auto get_degree_BN = [](pmBayesianNetwork *BN, vector<pmNode> ndList) -> int {
    int max = 0;
    int m;
    for (vector<pmNode>::iterator it = ndList.begin(); it != ndList.end(); ++it)
    {
      m = BN->getParents(*it).size();
      if (m > max)
      {
        max = m;
      }
    }
    return max;
  };

  // Returns a random number that follows the Laplace distribution
  auto generateur_Laplacien = [](double scale) -> double {
    int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    exponential_distribution<double> distribution(1 / scale);
    double e1 = distribution(generator);
    double e2 = distribution(generator);
    return e1 - e2;
  };

  std::vector<pmNode> ndList;
  this->orderNodes(this->get_graph(), ndList, true);
  ////////////SCALE/////////////
  int d = this->getVariables().size();
  int k = get_degree_BN(this, ndList);
  size_t n = DataDscptr->get_n_records();
  double eps = epsilon;
  double sc = (2 * (d - k)) / (n * eps);

  plProbValue bruit;
  ///////////SCALE//////////////

  plDistributionTable compObj;
  plProbTable compObjParent;
  plJointDistribution jd = this->getJointDistribution();
  plDistribution distribCourante;
  plVariablesConjunction varConjCourante;
  plVariable varCourante;
  plVariablesConjunction parentsCourants;
  vector<plVariable> todo;

  for (vector<pmNode>::iterator it = ndList.begin(); it != ndList.end(); ++it)
  {
    varCourante = this->getVariable(*it);
    parentsCourants = this->getParents(*it);

    varConjCourante = varCourante ^ parentsCourants;
    distribCourante = jd.ask(varConjCourante, PL_NO_OPTIMIZATION);

    vector<plProbValue> vectP;
    distribCourante.tabulate(vectP);

    if (parentsCourants.size() > 0)
    {
      for (vector<plProbValue>::iterator itbis = vectP.begin(); itbis != vectP.end(); ++itbis)
      {
        bruit = plProbValue(plFloat(generateur_Laplacien(sc)));
        *itbis += bruit;
        if (*itbis < 0)
        {
          *itbis = -*itbis;
        }
      }

      plDistribution distribCouranteNoisy = plDistribution(varConjCourante, vectP, false);

      vector<plProbValue> vectPNoisy;
      distribCouranteNoisy.tabulate(vectPNoisy);

      plCndDistribution distriDependance;
      plJointDistribution jdCourant = plJointDistribution(distribCouranteNoisy);

      varConjCourante = varCourante ^ parentsCourants;
      vector<plProbValue> vectPNoisyCND;
      jdCourant.ask(distriDependance, varCourante, parentsCourants);

      plCndDistribution distriNorm;
      distriDependance.compile(distriNorm);
      distriNorm.tabulate(vectPNoisyCND);

      compObj = plDistributionTable(varCourante, parentsCourants);

      plDistributionTable finale = plDistributionTable(varCourante, parentsCourants, vectPNoisyCND);
      plValues itParents(parentsCourants);
      plDistribution distriFinale = finale.instantiate(itParents, true);

      if (distriFinale.is_null())
      {
        int n = varCourante.cardinality();
        vector<plProbValue> uniform;
        for (int i = 0; i < n; ++i)
        {
          uniform.push_back(1);
        }
        compObj.push(plProbTable(varCourante, uniform), itParents);
      }
      else
      {
        compObj.push(distriFinale, itParents);
      }
      while (itParents.next())
      {
        distriFinale = finale.instantiate(itParents, true);
        if (distriFinale.is_null())
        {
          int n = varCourante.cardinality();
          vector<plProbValue> uniform;
          for (int i = 0; i < n; ++i)
          {
            uniform.push_back(1);
          }
          compObj.push(plProbTable(varCourante, uniform), itParents);
        }
        else
        {
          compObj.push(distriFinale, itParents);
        }
      }
      this->setComputableObject(*it, pmBayesianNetwork::copyComputableObject(compObj));
      vector<plVariable> vectParents = parentsCourants.as_variable_vector();

      if (!todo.empty())
      {
        vector<plVariable> todoBis = todo;
        int k = 0;
        for (vector<plVariable>::iterator itTer = todo.begin(); itTer != todo.end(); ++itTer)
        {
          if (contains(vectParents, *itTer))
          {
            plDistribution distriParent;
            vector<plProbValue> vectProbaParent;

            jdCourant.ask(distriParent, *itTer, PL_NO_OPTIMIZATION);
            distriParent.tabulate(vectProbaParent);

            plProbTable parentsFinale = plProbTable(*itTer, vectProbaParent);
            if (parentsFinale.is_null())
            {
              int n = varCourante.cardinality();
              vector<plProbValue> uniform;
              for (int i = 0; i < n; ++i)
              {
                uniform.push_back(1);
              }
              compObjParent = plProbTable(*itTer, uniform);
            }
            else
            {
              compObjParent = plProbTable(*itTer, vectProbaParent);
            }

            this->setComputableObject(this->getNode(*itTer), pmBayesianNetwork::copyComputableObject(compObjParent));

            todoBis.erase(todoBis.begin() + k);
          }
          else
          {
            k++;
          }
        }
        todo = todoBis;
      }
    }
    else
    {
      todo.push_back(varCourante);
    }
  }
}

/*
 * \brief Prints the graph of the bayesian network
 */
void pmBayesianNetwork::print_graph()
{
  unsigned int i, j, n;
  bool exists;
  pmEdge e;
  const pmGraph &graph = this->get_graph();
  n = (unsigned int)boost::num_vertices(graph);
  for (i = 0; i < n; i++)
  {
    cout << this->getVariables()[i].name() << " -->  ";
    for (j = 0; j < n; j++)
    {
      if (j == i)
        continue;
      boost::tie(e, exists) = edge(vertex(i, graph), vertex(j, graph), graph);
      if (exists)
        cout << this->getVariables()[j].name() << " ";
    }
    cout << endl;
  }
}

/*
 * \brief Prints the graph differences between two bayesian networks
 * \param pWithRespectToBN the bayesian network used for the comparison
 */
bool pmBayesianNetwork::printStructureDifferences(
    pmBayesianNetwork *pWithRespectToBN)
{
  pmEdgeIterator ei, ei_end;
  pmEdge e;
  pmNode node, parent, nodeRBN, parentRBN;
  unsigned int idxNode, idxParent;
  bool isEdgePresent, anyChange;

  anyChange = false;
  cout << "Added edges:" << endl;
  for (boost::tie(ei, ei_end) = boost::edges(this->graph);
       ei != ei_end;
       ++ei)
  {
    node = boost::target(*ei, this->graph);
    parent = boost::source(*ei, this->graph);
    idxNode = this->getVariableIndex(node);
    idxParent = this->getVariableIndex(parent);
    boost::tie(e, isEdgePresent) = edge(
        vertex(idxParent, pWithRespectToBN->graph),
        vertex(idxNode, pWithRespectToBN->graph),
        pWithRespectToBN->graph);
    if (!isEdgePresent)
    {
      cout << idxNode + 1 << " | " << idxParent + 1 << endl;
      anyChange = true;
    }
  }

  cout << "Removed edges:" << endl;
  for (boost::tie(ei, ei_end) = boost::edges(pWithRespectToBN->graph);
       ei != ei_end;
       ++ei)
  {
    node = boost::target(*ei, pWithRespectToBN->graph);
    parent = boost::source(*ei, pWithRespectToBN->graph);
    idxNode = pWithRespectToBN->getVariableIndex(node);
    idxParent = pWithRespectToBN->getVariableIndex(parent);
    boost::tie(e, isEdgePresent) = edge(
        vertex(idxParent, this->graph),
        vertex(idxNode, this->graph),
        this->graph);
    if (!isEdgePresent)
    {
      cout << idxNode + 1 << " | " << idxParent + 1 << endl;
      anyChange = true;
    }
  }
  return anyChange;
}

/************************************************************************/
/* After discussion with Mr. Francois and experimental results,
 * we conclude that there are two valid formulas for Monte Carlo estimation of Kl - Divergence
 * Sum_{x drawn from P(x) [N times]} log (P(x) / P2(x)) / N.
 * and
 * [ Sum_{x drawn according to a uniform distribution [N times]} P(x) log (P(x) / P2(x)) ] * (nbConf of x) / N.
/************************************************************************/
double pmBayesianNetwork::computeKLDivergence(pmBayesianNetwork &BN_compared,
                                              bool usingMCMCEstimation = false)
{
  // Read original network
  plJointDistribution jointDist_OrigBN = this->getJointDistribution();
  // Computed BN
  plJointDistribution jointDist_CompBN = BN_compared.getJointDistribution();

  // get number of variables
  plVariablesConjunction variables = this->getVariables();

  plValues values(variables);
  plProbValue p_originalBN = 0;
  plProbValue p_comparedBN = 0;
  plProbValue divergence = 0;

  ///////////////////////////////////////////////////////////////////////////////
  /** formula for KL-divergence
   ** D(P||Q) = Sum_x (P(x) log (P(x)/Q(x)))
   ** P = original pr
   ** Q = computed pr
  */
  if (!usingMCMCEstimation)
  {
    values.reset();
    do
    {
      try
      {
        plError::ignore_this_message(32, true);
        p_originalBN = jointDist_OrigBN.compute(values);
        plError::ignore_this_message(32, true);
        p_comparedBN = jointDist_CompBN.compute(values);

        if (p_comparedBN != 0 && p_originalBN != 0)
        { // to avoid zero values
          divergence += p_originalBN * log(p_originalBN / p_comparedBN);
        }
      }
      catch (...)
      {
        cout << "." << endl;
      }
    } while (values.next());

    cout << "\n KL_ complete: " << divergence << endl;
    return divergence;
  }
  else
  {
    double nbConf = variables.cardinality();
    plUniform pUniform(variables);

    plProbValue sum_divergence = 0;

    divergence = 0;

    values.reset();
    int k = 20000;
    for (int i = 0; i < k; i++)
    {
      plValues evidence(values);
      pUniform.draw(evidence);

      p_comparedBN = jointDist_CompBN.compute(evidence);

      p_originalBN = jointDist_OrigBN.compute(evidence);

      /// check for infinity error log (0) = infinity
      if ((p_comparedBN != 0) && (p_originalBN != 0))
      {
        divergence += p_originalBN * log(p_originalBN / p_comparedBN);
      }
    } // endfor

    divergence = divergence * ((float)nbConf / k);
    cout << " >>> " << k << "\t" << divergence << "\n";

    return (double)divergence; // sum and divide over samples to get average
  }                            // endelse
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool pmBayesianNetwork::isExistZYedgeAndZnotParentOfX(pmNode &x,
                                                      pmNode &y,
                                                      pmEdge *xyEdge,
                                                      pmGraph &graphArg)
{
  pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
  for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, graph);
       inEdgeItr != inEdgeItrEnd;
       ++inEdgeItr)
  {
    pmNode w = boost::source(*inEdgeItr, graphArg);
    if (*inEdgeItr != *xyEdge && !isParent(w, x, graph, NULL))
    {
      // if z != x(means it is not the same edge) and z is not a parent of x
      return true;
    }
  } // endfor
  return false;
}

void pmBayesianNetwork::labelEdges(pmEdge *xyEdge,
                                   pmNode &y,
                                   pmGraph &graph,
                                   std::string label,
                                   bool onlyUnknownEdges,
                                   bool incidentEdgesToo)
{
  boost::property_map<pmGraph, edge_name_t>::type edgeNameMap = get(edge_name, graph);
  boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);

  int w;
  if (label.compare("COMPELLED") == 0)
  {
    w = 2;
  }
  else
  {
    w = -1;
  }

  if (xyEdge != NULL)
  {
    edgeNameMap[*xyEdge] = label;
    weightmap[*xyEdge] = w;
  }

  if (incidentEdgesToo)
  {
    // find edges incident of y
    pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
    for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, graph);
         inEdgeItr != inEdgeItrEnd;
         ++inEdgeItr)
    {
      if (onlyUnknownEdges)
      {
        if (edgeNameMap[*inEdgeItr].compare("UNKNOWN") == 0)
        {
          edgeNameMap[*inEdgeItr] = label;
          weightmap[*inEdgeItr] = w;
        }
      }
      else
      {
        edgeNameMap[*inEdgeItr] = label;
        weightmap[*inEdgeItr] = w;
      }
    } // endfor
  }
}

// e
void pmBayesianNetwork::orderNodes(pmGraph &graph,
                                   std::vector<pmNode> &ndList,
                                   bool reverseOrder)
{
  // topological sort will gives the order from lowest to highest
  // we need the highest node toward lowest so, we will reverse the order and
  // then we will check the first(highest) node towards lowest by using
  // out_edges_list.
  ndList.clear();

  try
  {
    topological_sort(graph, std::back_inserter(ndList));
  }
  catch (not_a_dag &e)
  {
    cout << " \n Cycle found" << endl;
  }

  if(reverseOrder)
    std::reverse(ndList.begin(), ndList.end());
}

std::vector<pmEdge> pmBayesianNetwork::orderEdges(pmGraph &graph, 
                                                  std::vector<pmNode> &ndList,
                                                  bool reverseOrder) {
  
  std::vector<pmEdge> ordered_edges;

  boost::property_map<pmGraph, boost::vertex_index_t>::type g_vertices_idx = get(vertex_index, graph);

  unsigned int x_id, y_id, x_id_to_keep;

  // Make a map as follows: node -> order (useful later)
  std::map<pmNode, int> node_and_order;
  for (unsigned int i = 0; i < ndList.size(); i++)
    node_and_order[ndList[i]] = i;

  int highest_order = -1;
  pmNode x, y, x_to_keep;
  bool has_unordered_edge = true;
  bool graph_has_unordered_edge = true;

  pmEdgeIterator ei, ei_end;   // Iterating over all edges to order them         
  pmInEdgeIterator in_e, in_e_end; // Iterating over  in_edges for each node
  pmInNodeIterator in_n, in_n_end;

  pmEdge edge, edge_test;
  bool osef;

  boost::property_map<pmGraph, edge_name_t>::type name = get(edge_name, graph);

  // 0- Label every edge as "UNORDERED" 
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
    name[*ei] = "UNORDERED";
  }

  // 1- is already done in orderNodes();
  // 2- no need to set i (insertion order will be topological order)
  do {
    graph_has_unordered_edge = false;

    for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
      
      // 3- If the edge is unordered (= if there are still unordered edges in the graph)
      if(name[*ei].compare("UNORDERED") == 0) {
       
        graph_has_unordered_edge = true;

        // 4- [Find the lowest ordered y node] which has an unordered edge incident to it
        // (since the nodeList is ordered, we can just iterate over it like this)
        for (auto it_node_y = ndList.begin(); it_node_y != ndList.end(); ++it_node_y) {
          boost::tie(in_e, in_e_end) = in_edges(*it_node_y, graph);
          y_id = g_vertices_idx[*it_node_y];

          // If the node has parents
          if(in_e != in_e_end) {
            has_unordered_edge = false;
            
            // 4- Find the lowest ordered y node [which has an unordered edge incident to it]
            for (; in_e != in_e_end; ++in_e)
              if(name[*in_e].compare("UNORDERED") == 0)
                has_unordered_edge = true;
            
            if(has_unordered_edge) {
              boost::tie(in_n, in_n_end) = parents(*it_node_y);
              
              // Pre 5- Take the first parent (x) node (for which x -> y is not ordered) as the highest ordered node
              // Note: this do/while() will probably end up being poorly written
              if (in_n != in_n_end) {
                highest_order = -1;

                // 5- Find the highest order X node for which x -> y is not ordered
                for (; in_n != in_n_end; ++in_n) {
                  x = *in_n;

                  x_id = g_vertices_idx[x];
                  boost::tie(edge_test, osef) = boost::edge(vertex(x_id, graph), vertex(y_id, graph), graph);
                  
                  if (name[edge_test].compare("UNORDERED") == 0 && node_and_order.find(x)->second > highest_order) {
                    highest_order = node_and_order.find(x)->second;
                    x_id = g_vertices_idx[x];
                    boost::tie(edge, osef) = boost::edge(vertex(x_id, graph), vertex(y_id, graph), graph);
                    x_to_keep = *in_n;
                    x_id_to_keep = g_vertices_idx[x];
                  }
                }
              }

              // Add the edge to the ordered edges list and mark it as being ordered
              boost::tie(edge, osef) = boost::edge(vertex(x_id_to_keep, graph), vertex(y_id, graph), graph);
              ordered_edges.push_back(edge);
              name[edge] = "ORDERED";
            }
          }
        }
      }
    }
  } while(graph_has_unordered_edge);

  if(reverseOrder)
    std::reverse(ordered_edges.begin(), ordered_edges.end());

  return ordered_edges;
}

bool pmBayesianNetwork::isParent(pmNode &w,
                                 pmNode &y,
                                 pmGraph &graph,
                                 pmEdge *wyEdge)
{
  bool enteredtheforloop = false;
  // cout << "-------------------isParent-- Entered in the function isParent() when the two nodes are " << getVariable(w) << " and " << getVariable(y) << endl;
  pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
  for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, graph);
       inEdgeItr != inEdgeItrEnd;
       ++inEdgeItr)
  {
    // cout << "--------------------isParent-- "
    //      << "boost is " << getVariable(boost::source(*inEdgeItr, graph)) << endl;
    enteredtheforloop = true;
    // cout << "--------------------isParent-- entered in the for loop" << endl;
    if (boost::source(*inEdgeItr, graph) == w)
    { // if w is a parent of y
      pmEdge wyE = *inEdgeItr;
      wyEdge = &wyE;
      // cout << "\n-------------------isParent-- will return true from the function isParent" << endl;
      return true;
    }
  }
  wyEdge = NULL;
  return false;
}

void pmBayesianNetwork::print_CPDAG(pmGraph graph)
{
  //pmEdge e;
  property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
  property_map<pmGraph, edge_name_t>::type edgeLabel = get(edge_name, graph);

  pmEdgeIterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
  {
    if (weightmap[*ei] == 2)
      std::cout << vertex_id[boost::source(*ei, graph)] << " --> "
                << vertex_id[boost::target(*ei, graph)] << " " << std::endl;
    else
      std::cout << vertex_id[boost::source(*ei, graph)] << " ----  "
                << vertex_id[boost::target(*ei, graph)] << " " << std::endl;
  }
}

/* Algorithm DAG-to-CPDAG, chickering(1995)
 * Input: DAG graph
 * OutPut: DAG graph with each edge labeled either "COMPELLED" or "REVERSIBLE"
 *
 * Order the edges in Graph
 * Label every edge in Graph as "UNKNOWN"
 * While there are edges labeled "UNKNOWN" in Graph
 *      let x->y be the lowest ordered edge that is labeled "UNKNOWN"
 *      For every edge w->x labeled "COMPELED"
 *          IF w is not a parent of y then
 *             label x->y and every edge  incident into y with "COMPELLED"
 *             GO TO While;
 *          ELSE
 *             Label w-> with "COMPELLED"
 *
 *  IF there exists an edge z->y such that z!=x and z is not a parent of x then
 *      Label x->y and all "UNKNOWN" edges incident into y with "COMPELLED"
 *  ELSE
 *      Label x->y and all "UNKNOWN" edges incident into y with "REVERSIBLE"
 **/
pmGraph pmBayesianNetwork::convertDAGintoCPDAG(pmGraph graph)
{
  boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
  boost::property_map<pmGraph, edge_name_t>::type name = get(edge_name, graph);

  // 1-order nodes using topological sort
  this->orderNodes(graph, nodeList, true);

  // 1.5-order the edges in the topological order
  std::vector<pmEdge> graph_edges = orderEdges(graph, nodeList, false);

  // 2-label every edge of graph as "UNKNOWN"
  pmEdgeIterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
  {
    name[*ei] = "UNKNOWN";
    weightmap[*ei] = -1;
  } // end for

  // 3-
  std::vector<pmNode>::iterator nodeListItr;
  pmNode x, y, w, z;
  pmEdge *xyEdge = NULL;

  for (nodeListItr = nodeList.begin();
       nodeListItr != nodeList.end();
       ++nodeListItr)
  {
    bool flag = false;
    x = *nodeListItr;
    pmOutEdgeIterator outEdgeItr, outEdgeItrEnd;
    for (boost::tie(outEdgeItr, outEdgeItrEnd) = out_edges(x, graph);
         outEdgeItr != outEdgeItrEnd;
         ++outEdgeItr)
    { // to get x-->y edges
      y = boost::target(*outEdgeItr, graph);
      pmEdge xyE = *outEdgeItr;
      xyEdge = &xyE; // out_edges, it is already sorted from lowest to highest

      name = get(edge_name, graph);

      if (name[*outEdgeItr].compare("UNKNOWN") == 0)
      {
        // for every edge w-->x labeled "compelled"
        pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
        for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(x, graph);
             inEdgeItr != inEdgeItrEnd;
             ++inEdgeItr)
        {
          if (name[*inEdgeItr].compare("COMPELLED") == 0)
          {
            w = boost::source(*inEdgeItr, graph);
            pmEdge *wyEdge = NULL;
            if (!isParent(w, y, graph, wyEdge))
            {
              labelEdges(xyEdge, y, graph, "COMPELLED", false, true);
              flag = true;
              break;
            }
            else
            {
              labelEdges(wyEdge, y, graph, "COMPELLED", false, false);
            }
          } // endif
        }   // endfor
        if (!flag)
        {
          if (isExistZYedgeAndZnotParentOfX(x, y, xyEdge, graph))
          {
            labelEdges(xyEdge, y, graph, "COMPELLED", true, true);
          }
          else
          {
            labelEdges(xyEdge, y, graph, "REVERSIBLE", true, true);
          }
        }
      } // endif
    }   // end for
    name = get(edge_name, graph);
  } // end for

  return graph;
}

/*
 * \brief Compares the CPDAG of two bayesian networks
 *
 */
void pmBayesianNetwork::compareBNs(pmGraph &origG,
                                   pmGraph &computedG,
                                   unsigned int &TP,
                                   unsigned int &FP,
                                   unsigned int &TN,
                                   unsigned int &FN,
                                   unsigned int &strHammingDistance)
{
  // To find the True positives, False positives, True negatives,
  // False negatives, Reversed edges and structural hamming distance
  // (see MMHC orignal paper).
  TP = FP = TN = FN = strHammingDistance = 0;
  pmGraph origGraph = convertDAGintoCPDAG(origG);
  pmGraph computedGraph = convertDAGintoCPDAG(computedG);
  //=====================================================

  cout << "CPDAG -- ORIGINAL" << endl;
  print_CPDAG(origGraph);

  cout << "CPDAG -- LEARNED" << endl;
  print_CPDAG(computedGraph);

  /* Number of nodes within bayesian network */
  int numberOfNodes = boost::num_vertices(origGraph);

  property_map<pmGraph, edge_weight_t>::type weightMapOrig = boost::get(edge_weight, origGraph);
  property_map<pmGraph, edge_weight_t>::type weightMapComp = boost::get(edge_weight, computedGraph);

  std::vector<pair<unsigned int, unsigned int>> origGraphEdgeList;
  std::vector<pair<unsigned int, unsigned int>> compGraphEdgeList;
  pmEdgeIterator ei, ei_end;
  pmEdge compEdgeItr, compEdgeItr2;
  bool compTestItr, compTestItr2;

  // To compute SHD, first get the list of the edges for both graph, then
  // delete the common/same edges, the remaining number of elements
  // will be difference
  for (boost::tie(ei, ei_end) = boost::edges(computedGraph);
       ei != ei_end;
       ++ei)
  {
    compGraphEdgeList.push_back(make_pair(
        vertex_id[boost::source(*ei, computedGraph)],
        vertex_id[boost::target(*ei, computedGraph)]));
  }

  // First delete the same edges and having the same weight in computed
  //   edge list
  // Then find if edge a->b is already exist in computed edge as reverse
  //   edge b->a then dont save it.
  // Finally sum of the size of both list will be the SHD.

  for (boost::tie(ei, ei_end) = boost::edges(origGraph); ei != ei_end; ++ei)
  {
    // cout << "\n==========New Iteration in the loop for SHD==========\n"
    //      << endl;
    unsigned int src = vertex_id[boost::source(*ei, origGraph)];
    unsigned int trgt = vertex_id[boost::target(*ei, origGraph)];

    boost::tie(compEdgeItr, compTestItr) = boost::edge(vertex(src, computedGraph),
                                                       vertex(trgt, computedGraph),
                                                       computedGraph);
    boost::tie(compEdgeItr2, compTestItr2) = boost::edge(vertex(trgt, computedGraph),
                                                         vertex(src, computedGraph),
                                                         computedGraph);
    //------------------------------------------------------------------------
    pair<unsigned int, unsigned int> edgeNodesPair = make_pair(src, trgt);

    std::vector<pair<unsigned int, unsigned int>>::iterator itr = find(
        compGraphEdgeList.begin(),
        compGraphEdgeList.end(),
        edgeNodesPair);
    //------------------------------------------------------------------------
    // cout << "CompGraphEdgeList at the beginning is: (Each pair in line)" << endl;
    // for (int i = 0; i < compGraphEdgeList.size(); i++)
    // {
    //   cout << compGraphEdgeList[i].first << ", " << compGraphEdgeList[i].second << endl;
    // }
    // cout << "=========\n"
    //      << endl;
    //------------------------------------------------------------------------
    pair<unsigned int, unsigned int> edgeNodesPairReversed = make_pair(trgt, src);

    std::vector<pair<unsigned int, unsigned int>>::iterator itr2 = find(
        compGraphEdgeList.begin(),
        compGraphEdgeList.end(),
        edgeNodesPairReversed);
    //------------------------------------------------------------------------
    int EdgeWeightInOrig = weightMapOrig[*ei];
    int EdgeWeightInComp = compTestItr ? weightMapComp[compEdgeItr] : (compTestItr2 ? weightMapComp[compEdgeItr2] : 0);
    bool equalWeights = (EdgeWeightInOrig == EdgeWeightInComp);

    bool edgeDirectedInOrig = (weightMapOrig[*ei] == 2);
    bool edgeUndirectedInOrig = (weightMapOrig[*ei] == -1);

    bool eraseDirected = (itr != compGraphEdgeList.end() && edgeDirectedInOrig);
    bool eraseUndirected1 = (itr != compGraphEdgeList.end() && edgeUndirectedInOrig);
    bool eraseUndirected2 = (itr2 != compGraphEdgeList.end() && edgeUndirectedInOrig);

    bool InverseEdgeFoundInCompWithSameWeightInOrig = (itr2 != compGraphEdgeList.end() && equalWeights);

    bool EdgeOrInverseFoundInComp = (itr != compGraphEdgeList.end() || itr2 != compGraphEdgeList.end());
    bool EdgeOrInverseFoundInCompAndDiffWeightsThanOrig = (EdgeOrInverseFoundInComp &&
                                                           !equalWeights);

    // cout << "eraseDirected is " << eraseDirected << endl;
    // cout << "eraseUndirected1 is " << eraseUndirected1 << endl;
    // cout << "eraseUndirected2 is " << eraseUndirected2 << endl;
    // cout << "weightMapOrig[*ei] is " << weightMapOrig[*ei] << endl;
    // if (compTestItr)
    //   cout << "weightMapComp[*itr]" << weightMapComp[compEdgeItr] << endl;
    // if (compTestItr2)
    //   cout << "weightMapComp[*itr2]" << weightMapComp[compEdgeItr2] << endl;
    // cout << "EQUAL WEIGHTS =====>" << equalWeights << endl;

    /*il est directed dans origGrah, et présent et directed dans le même sens dans ComputedGraph ==> on l'enlève de computeGraph --> eraseDirected*/
    /*il est undirected dans origGraph et undirected dans ComputedGraph ==> on l'enlève de computedGraphList --> eraseUndirected*/
    if (equalWeights && (eraseDirected || eraseUndirected1))
    {
      compGraphEdgeList.erase(itr);
      // cout << "iterator was erased to compGraphEdgeList" << endl;
    }
    else if (equalWeights && (eraseUndirected2))
    {
      compGraphEdgeList.erase(itr2);
      // cout << "iterator2 was erased to compGraphEdgeList" << endl;
    }
    // if edge a->b is already exist in reverse direction in compGraphEdgelist
    // then no need to store it again.

    /*il est undirected dans origGraph et présent dirigé dans Computed dans un sens ou l'autre*/
    else if (InverseEdgeFoundInCompWithSameWeightInOrig ||
             EdgeOrInverseFoundInCompAndDiffWeightsThanOrig)
    {
      // cout << "Not added in OrigEdgeList" << endl;
      continue;
    }
    else
    {
      // cout << "edgeNodesPair added to the origGraphEdgeList" << endl;
      origGraphEdgeList.push_back(edgeNodesPair);
    }
  } // end for

  // cout << "\n=========" << endl;
  // cout << "origGraphEdgeList is: (Each pair in line)" << endl;
  // cout << "origGraphEdgeList size is : " << origGraphEdgeList.size() << endl;
  // for (int i = 0; i < origGraphEdgeList.size(); i++)
  // {
  //   cout << origGraphEdgeList[i].first << ", " << origGraphEdgeList[i].second << endl;
  // }
  // cout << "=========\n"
  //      << endl;

  // cout << "\n=========" << endl;

  // // cout << "CompGraphEdgeList a la fin is: (Each pair in line)" << endl;
  // for (int i = 0; i < compGraphEdgeList.size(); i++)
  // {
  //   cout << compGraphEdgeList[i].first << ", " << compGraphEdgeList[i].second << endl;
  // }
  // cout << "=========\n"
  //      << endl;

  strHammingDistance = compGraphEdgeList.size() + origGraphEdgeList.size();

  /* Variables representing two nodes into the following loops */
  /* Scrolling through nodes */
  // cout<<"Second For loop, 2 for loops inside each other to calculate TP, FP, TN, FN"<<endl;
  for (int i = 0; i < numberOfNodes - 1; i++)
  {
    /* Scrolling through remaining nodes */
    for (int j = i + 1; j < numberOfNodes; j++)
    {
      pmEdge edgeA, edgeB;
      bool testA = false, testB = false;

      /* Variables association to desired edges in original graph*/
      boost::tie(edgeA, testA) = boost::edge(vertex(i, origG),
                                             vertex(j, origG),
                                             origG);
      boost::tie(edgeB, testB) = boost::edge(vertex(j, origG),
                                             vertex(i, origG),
                                             origG);
      pmEdge compEdgeA, compEdgeB;
      bool compTestA = false, compTestB = false;

      /* Variables association to desired edges in computed graph*/
      boost::tie(compEdgeA, compTestA) = boost::edge(vertex(i, computedG),
                                                     vertex(j, computedG),
                                                     computedG);
      boost::tie(compEdgeB, compTestB) = boost::edge(vertex(j, computedG),
                                                     vertex(i, computedG),
                                                     computedG);
      // cout << "compTestA : " << compTestA << " and compTestB: " << compTestB << endl;

      // cout<<"Source is "<<i<<" and target is "<<j<<endl;
      // cout<<"testA "<<testA<<endl;
      // cout<<"testB "<<testB<<endl;
      // cout<<"compTestA "<<compTestA<<endl;
      // cout<<"compTestB "<<compTestB<<endl;
      if ((testA || testB) && (compTestA || compTestB))
      {
        ++TP;
      }
      else if ((!testA && !testB) && (compTestA || compTestB))
      {
        ++FP;
      }
      else if ((!testA && !compTestA) && (!testB && !compTestB))
      {
        ++TN;
      }
      else if ((testA || testB) && (!compTestA && !compTestB))
      {
        ++FN;
      }
    }
  }
}
/**********************************************************************************************/
/* Begin Bouthayna Ed-dahmouni : Serialization & unserialization of NoisyMax bayesian networks*/
/**********************************************************************************************/

// =============================== Serialization ================================
/**
 * \brief serializes a noisyMax bayesian network (inspired from the ProBT doc)
 * and saves data in a xml file
**/
void pmBayesianNetwork::save_as_xml(
    const std::string &file_name, std::string jd_name)
{

  // Serialization
  std::string ser_save = "serial_s";
  plSerializer serial_save(ser_save);

  // Add joint distribution to serialisation
  plJointDistribution jd = this->getJointDistribution();
  std::string bn_jd = jd_name;
  serial_save.add_object(bn_jd, jd);

  // Add Learn Objects to serialisation
  vector<plLearnObject *> vect_lo = this->getLearnObjectList();
  if (vect_lo[0] != 0)
  {
    std::string bn_lo = "Learn Object";
    plLearnDistributions lo = plLearnDistributions(vect_lo, jd.get_variables());
    serial_save.add_object(bn_lo, lo);
  }

  //Creation of the serialised file
  std::ofstream serialFile(file_name);
  if (serialFile)
  {
    try
    {
      serial_save.save(serialFile);
    }
    catch (const std::exception &e)
    {
      std::cerr << " Error while writing in serialization file : " << e.what();
    }
  }
  else
  {
    std::cerr << " Error : cannot create serialization file .\n ";
  }
}

// ========================== Unserialization ================================

/**
 * \brief Loads and returns a noisyMax Bayesian Network from a xdsl file
**/
pmBayesianNetwork pmBayesianNetwork::load_from_xml(
    const std::string &file_name, std::string jd)
{
  // Create Serializer
  plSerializer serial_load("serial_l");

  // Load bayesian network
  plJointDistribution loaded_jd;
  std::string jd_name = jd;

  // Create and write in serialization file
  std::ifstream serialFile(file_name);
  if (serialFile)
  {
    try
    {
      serial_load.load(file_name);
      serial_load.get_object(jd_name, loaded_jd);
    }

    catch (const std::exception &e)
    {
      std::cerr << " Error while loading from zerialisation file : " << e.what();
    }
  }
  else
  {
    std::cerr << " Error : cannot read serialization file .\n ";
  }

  // Creation of the bayesian network
  pmBayesianNetwork bn_serialized(loaded_jd);
  bn_serialized.summary();
  return bn_serialized;
}

vector<string> pmBayesianNetwork::create_test_values()
{
  std::vector<std::string> result;

  for (int i = 0; i < this->getVariables().size(); i++)
  {
    plVariable vi(this->getVariables()[i]);
    plDistribution variable = this->query(vi).compile();

    int score = rand() % 1000;
    int score_min = 0;
    string type_retenu;
    for (int j = 0; j < vi.get_type().cardinality(); j++)
    {
      if (score <= (variable.compute(j) * 1000) + score_min && score > score_min)
      {
        type_retenu = variable.get_variables()[0].get_type().get_values_as_strings()[j];
      }
      score_min = score_min + variable.compute(j) * 1000;
    }
    result.push_back(type_retenu);
  }
  return result;
}

void pmBayesianNetwork::generateRandomGraph(unsigned int nbNodes, unsigned int maxInducedWidth, unsigned int nbIterations, unsigned int maxNodeDegree, unsigned int maxNbEdges)
{
  int numberOfNodes = boost::num_vertices(this->graph);
  pmUtility ut;
  cout << "graph before generating a random graph" << endl;
  print_graph();
  cout << "=========================\n"
       << endl;
  ut.generatePMMixedDAG(&(this->graph), nbNodes, maxInducedWidth, nbIterations, maxNodeDegree, maxNbEdges);
  cout << "Random graph is: " << endl;
  print_graph();
}

/*********************************************************************************************/
/* End Bouthayna Ed-dahmouni : Serialization & unserialization of NoisyMax bayesian networks */
/*********************************************************************************************/
