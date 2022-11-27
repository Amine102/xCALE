#include "pilgrim/general/pm15dBN.h"

#include "pilgrim/general/pmDynamicBayesianNetwork.h"

#ifndef PILGRIM_DEBUG
    #define PILGRIM_DEBUG false
#endif
#ifndef PILGRIM_PM15DBN_LOG_PREFIX
    #define PILGRIM_PM15DBN_LOG_PREFIX "[pm15dBN.cpp] - "
#endif

using namespace boost;
using namespace std;
using namespace PILGRIM;

pm15dBN::pm15dBN()
    : pmBayesianNetwork() {
}

pm15dBN::pm15dBN(const plJointDistribution &jd)
    : pmBayesianNetwork(jd) {}


pm15dBN::pm15dBN(const plJointDistribution &jd, int arg_present, int arg_futur, vector<plVariable> arg_outgoingInterface)
    : pmBayesianNetwork(jd) {
    this->setPresent(arg_present);
    this->setFutur(arg_futur);
    this->setOutgoingInterface(arg_outgoingInterface);
}
pm15dBN::pm15dBN(const pm15dBN &dBN_15)
    : pmBayesianNetwork(dBN_15.getJointDistribution()) {
    this->setPresent(dBN_15.getPresent());
    this->setFutur(dBN_15.getFutur());
    this->setOutgoingInterface(dBN_15.getOutgoingInterface());
}

pm15dBN::pm15dBN(pm15dBN *bn)
    : pm15dBN(bn->getJointDistribution()) {
    this->setPresent(bn->getPresent());
    this->setFutur(bn->getFutur());

    vector<plVariable>     outgoingInterface;
    plVariablesConjunction vars      = bn->getVariables();
    vector<string>         varsNames = vars.get_names();

    string cname;
    string ending = "_t";

    for(std::vector<string>::iterator it = varsNames.begin(); it != varsNames.end(); ++it) {
        cname = *it;
        //si le nom est plus long que "_t"
        if(cname.length() > 2) {
            //si le nom ne finit pas par "_t"
            if(0 != cname.compare(cname.length() - ending.length(), ending.length(), ending)) {
                //alors il fait partie de l'outgoing interface
                outgoingInterface.push_back(vars.get_variable_with_name(cname));
            }
        } else {
            outgoingInterface.push_back(vars.get_variable_with_name(cname));
        }
    }

    this->outgoingInterface = outgoingInterface;
}

pm15dBN::pm15dBN(pmDynamicBayesianNetwork *dBN, plValues evidences)
    : pmBayesianNetwork(dBN->simpleGT()) {
    this->setPresent(0);
    this->setFutur((this->getPresent()) + 1);

    vector<plVariable>     outgoingInterface;
    plVariablesConjunction vars = this->getVariables();

    vector<string> varsNames = vars.get_names();

    string         cname;
    string         ending = "_t";
    pmEdgeIterator ei, ei_end;
    pmEdge         e_t;

    // Définition de l'OutgoingInterface
    for(boost::tie(ei, ei_end) = boost::edges(this->graph); ei != ei_end; ++ei) {
        e_t = *ei;

        pmNode     src         = boost::source(e_t, this->graph);
        plVariable srcVariable = this->getVariable(src);
        string     srcName     = srcVariable.name();
        // si le nom de la source ne finit pas par "_t"
        if(srcName.length() > 2) {
            if(0 != srcName.compare(srcName.length() - ending.length(), ending.length(), ending)) {
                pmNode     tgt_t         = boost::target(e_t, this->graph);
                plVariable tgtVariable_t = this->getVariable(tgt_t);
                string     tgtName_t     = tgtVariable_t.name();
                // si le nom de la cible fini par "_t"
                if(0 == tgtName_t.compare(tgtName_t.length() - ending.length(), ending.length(), ending)) {
                    outgoingInterface.push_back(vars.get_variable_with_name(srcName));
                }
            }
        }
    }

    this->outgoingInterface    = outgoingInterface;

    // Identification de la liste des edges entre des variable se terminant par _t
    set<string> variables_with_new_parents;

    pmNode     src_t, tgt_t;
    plVariable srcVariable_t, tgtVariable_t;
    string     srcName_t, tgtName_t;

    // Parcours de toutes les edges et, pour celles qui lient deux noeuds dont le nom se termine par "_t"
    // on verifie si le graphe contient deux variables ayant ces deux mêmes noms mais sans le "_t".
    // Si oui, alors on ajoute au graphe une nouvelle edge entre les deux variables sans le "_t".
    // A la fin on crée des nouveaux computables objects pour les noeuds ayant reçu des nouveaux parents
    for(boost::tie(ei, ei_end) = boost::edges(this->graph); ei != ei_end; ++ei) {
        e_t = *ei;

        src_t         = boost::source(e_t, this->graph);
        srcVariable_t = this->getVariable(src_t);
        srcName_t     = srcVariable_t.name();

        if(srcName_t.length() > 2)
            if(0 == srcName_t.compare(srcName_t.length() - ending.length(), ending.length(), ending)) {
                tgt_t         = boost::target(e_t, this->graph);
                tgtVariable_t = this->getVariable(tgt_t);
                tgtName_t     = tgtVariable_t.name();

                if(tgtName_t.length() > 2)
                    if(0 == tgtName_t.compare(tgtName_t.length() - ending.length(), ending.length(), ending)) {
                        //on verifie si pour les couples de variables (srcName_t, tgtName_t) le graphe contient deux variables (srcName, tgtName)
                        bool       srcName_found = false;
                        bool       tgtName_found = false;
                        plVariable srcVariable, tgtVariable;

                        string srcName = srcName_t.substr(0, srcName_t.length() - ending.length());
                        string tgtName = tgtName_t.substr(0, tgtName_t.length() - ending.length());

                        string cname;

                        for(std::vector<string>::iterator it = varsNames.begin(); it != varsNames.end(); ++it) {
                            if(srcName_found && tgtName_found)
                                break;

                            cname = *it;
                            if(!srcName_found) {
                                if(cname.compare(srcName) == 0) {
                                    srcName_found = true;
                                    srcVariable   = vars.get_variable_with_name(srcName);
                                }
                            }

                            if(!tgtName_found) {
                                if(cname.compare(tgtName) == 0) {
                                    tgtName_found = true;
                                    tgtVariable   = vars.get_variable_with_name(tgtName);
                                }
                            }
                        }

                        // Si les deux variable "srcName" et "tgtName" on été retrouvées alors on ajoute dans le graphe l'edge : (srcName, tgtName).
                        if(srcName_found && tgtName_found) {
                            pmNode src = this->getNode(srcVariable);
                            pmNode tgt = this->getNode(tgtVariable);
                            this->add_arrow(src, tgt);

                            // MAJ de la liste avec les noeuds qui on reçu un nouveau parent
                            variables_with_new_parents.insert(tgtName);
                        }
                    }
            }
    }


    // Parcours des noeuds qui ont reçu un nouveau parent
    // Création des computable objects qui leur correspondent (plCndDistribution avec des valeurs aléatoires)

    for(set<string>::iterator it = variables_with_new_parents.begin(); it != variables_with_new_parents.end(); ++it) {
        string     cName     = *it;
        plVariable cVariable = vars.get_variable_with_name(cName);
        pmNode     cNode     = this->getNode(cVariable);

        plVariablesConjunction cParents = this->getParents(cNode);

        plCndDistribution *cCompObj = new plCndDistribution(cVariable, cParents);

        this->setComputableObject(cNode, cCompObj);
    }

    //========= Init G0 ==========
    this->updateG0(evidences, dBN);
}


int pm15dBN::getPresent() const {
    return (this->present);
}

int pm15dBN::getFutur() const {
    return (this->futur);
}

void pm15dBN::setPresent(int t) {
    this->present = t;
}

void pm15dBN::setFutur(int t) {
    this->futur = t;
}

vector<plVariable> pm15dBN::getOutgoingInterface() const {
    return (this->outgoingInterface);
}

void pm15dBN::setOutgoingInterface(const vector<plVariable> &OI) {
    this->outgoingInterface = OI;
}

void pm15dBN::summary_1_5() {
    std::cout << "\n\nPresent : " << this->getPresent() << std::endl;
    std::cout << "Futur : " << this->getFutur() << std::endl;
    std::cout << "\nLes noeuds de l'outgoing Interface sont : ";

    vector<plVariable> OI = this->getOutgoingInterface();
    for(auto var : OI) {
        std::cout << var.name() << " | ";
    }

    std::cout << std::endl
              << std::endl;
    this->summary();
}

void pm15dBN::serialize(const std::string &uri_file, const std::string& jd_id, const std::string& OI_id) {
    // Add objects to save
    plSerializer serial_save("serial_s");
    this->serialize(serial_save, jd_id, OI_id);

    std::ofstream serialFile(uri_file);

    if(serialFile) {
        try {
            serial_save.save(serialFile);
        } catch(const std::exception &e) {
            std::cerr << PILGRIM_PM15DBN_LOG_PREFIX << "Error while writing in serialization file : " << e.what();
        }
    } else {
        std::cerr << PILGRIM_PM15DBN_LOG_PREFIX << "Error : cannot create serialization file.\n ";
    }
}


void pm15dBN::serialize(plSerializer& serial_save, const std::string& jd_id, const std::string& OI_id, const std::string& time_id) {
    if(PILGRIM_DEBUG)
        std::cout << PILGRIM_PM15DBN_LOG_PREFIX << "T=" << this->getPresent() << " next is: " << this->getFutur() << std::endl;
    plIntegerType present_futur_interval(this->getPresent(), this->getFutur());

    if(PILGRIM_DEBUG)
        std::cout << PILGRIM_PM15DBN_LOG_PREFIX << "Min Interval=" << present_futur_interval.get_min() << std::endl
                  << "\tMax Interval= " << present_futur_interval.get_max() << std::endl;

    plJointDistribution jd_1_5 = this->getJointDistribution();

    serial_save.add_object(jd_id, jd_1_5);
    serial_save.add_object(time_id, present_futur_interval);

    plVariable             tmp;
    vector<plVariable>     oi_vars = this->getOutgoingInterface();
    plVariablesConjunction conjuctionOI;

    if(!this->getOutgoingInterface().empty()) {
        for(auto& oi_var: oi_vars) {
            conjuctionOI ^= oi_var;
        }
    }

    serial_save.add_object(OI_id, conjuctionOI);
}

pm15dBN pm15dBN::unserialize(plSerializer serial_load, string jd_id, string OI_id, string time_id) {
    // Objects to get back from th XML file
    plJointDistribution loaded_1_5_jd;
    int                 min, max;
    plIntegerType       loadedIntegerType;
    vector<plVariable>  OI;

    try {
        plVariablesConjunction conjunctionOI;
        serial_load.get_object(OI_id, conjunctionOI);
        serial_load.get_object(jd_id, loaded_1_5_jd);

        pm15dBN returned_dBN_1_5(loaded_1_5_jd);
        serial_load.get_object(time_id, loadedIntegerType);
        min = loadedIntegerType.get_min();
        max = loadedIntegerType.get_max();
        returned_dBN_1_5.setPresent(min);
        returned_dBN_1_5.setFutur(max);

        conjunctionOI.get_all_variables(OI);
        returned_dBN_1_5.setOutgoingInterface(OI);
        return returned_dBN_1_5;
    } catch(const std::exception &e) {
        std::cerr << PILGRIM_PM15DBN_LOG_PREFIX << "Error while loading from zerialisation file : " << e.what();
        throw e;
    }
}

pm15dBN pm15dBN::unserialize(const std::string &file_name, std::string jd_id, std::string OI_id) {

    pm15dBN dBN_1_5;
    // Create Serializer
    plSerializer serial_load("serial_l");

    // Objects to get back from th XML file
    plJointDistribution loaded_1_5_jd;
    int                 min, max;
    plIntegerType       loadedIntegerType;
    vector<plVariable>  OI;

    // Create and write in serialization file
    std::ifstream serialFile(file_name);
    if(serialFile) {
        try {
            serial_load.load(file_name);

            plVariablesConjunction conjunctionOI;
            serial_load.get_object(OI_id, conjunctionOI);
            serial_load.get_object(jd_id, loaded_1_5_jd);

            pm15dBN returned_dBN_1_5(loaded_1_5_jd);
            serial_load.get_object("present_futur", loadedIntegerType);
            min = loadedIntegerType.get_min();
            max = loadedIntegerType.get_max();
            returned_dBN_1_5.setPresent(min);
            returned_dBN_1_5.setFutur(max);

            vector<plVariable>     finalConjunctionOI;
            plVariablesConjunction jdConjunction = returned_dBN_1_5.getVariables();
            for(auto var : conjunctionOI) {
                finalConjunctionOI.push_back(jdConjunction.get_variable_with_name(var.name()));
            }

            returned_dBN_1_5.setOutgoingInterface(finalConjunctionOI);
            return returned_dBN_1_5;
        } catch(const std::exception &e) {
            std::cerr << PILGRIM_PM15DBN_LOG_PREFIX << "Error while loading from zerialisation file : " << e.what();
        }
    } else {
        std::cerr << PILGRIM_PM15DBN_LOG_PREFIX << "Error : cannot read serialization file .\n ";
    }
    return dBN_1_5;
}

plComputableObjectList pm15dBN::query_JT(plVariablesConjunction target_variable, plValues evidence, bool recupEv) {
    plVariablesConjunction right_var;
    plComputableObjectList result;
    plValues               new_ev;
    plCndJtDistribution    CndP;
    plVariable             temp;

    plJunctionTree JT       = plJunctionTree(this->getJointDistribution());
    vector<string> ev_names = evidence.get_variables().get_names();

    if(!recupEv) {
        for(unsigned int i = 0; i < evidence.get_variables().size(); i++) {
            temp      = this->getVariables().get_variable_with_name(ev_names[i]);
            right_var = right_var ^ temp;
            new_ev    = new_ev.add(temp, evidence[evidence.get_variables()[i]]);
        }
    } else {
        new_ev    = evidence;
        right_var = evidence.get_variables();
    }

    for(auto target : target_variable) {
        if(evidence.get_variables().contains(target)) {
            // If the current target is also an evidence, recreate the table from the plValues
            vector<plProbValue> table;
            unsigned int        value_index;
            target.get_type().get_value_index(value_index, evidence[target]);

            for(unsigned int v = 0; v < target.get_type().cardinality(); v++) {
                table.push_back(v == value_index);
            }

            result *= plDistribution(target, table);
        } else {
            plDistribution P;
            plDistribution PNorm;
            CndP = JT.ask(target, right_var);
            CndP.instantiate(P, new_ev);
            P.compile(PNorm);

            result *= PNorm;
        }
    }
    return result;
}
plComputableObjectList pm15dBN::query_JT(plVariablesConjunction target_variable) {
    plVariablesConjunction right_var;
    plComputableObjectList result;
    plValues               new_ev;
    plJtDistribution       P;
    plVariable             temp;

    plJunctionTree JT = plJunctionTree(this->getJointDistribution());

    for(auto target : target_variable) {
        plDistribution PNorm;
        P = JT.ask(target);
        P.compile(PNorm);

        result *= PNorm;
    }
    return result;
}
plComputableObjectList pm15dBN::query_JD(plVariablesConjunction target_variable, plValues evidence, bool recupEv) {
    plVariablesConjunction right_var;
    plComputableObjectList result;
    plValues               new_ev;
    plCndDistribution      CndP;

    plVariable     temp;
    vector<string> ev_names = evidence.get_variables().get_names();
    if(!recupEv) {
        for(unsigned int i = 0; i < evidence.get_variables().size(); i++) {
            temp      = this->getVariables().get_variable_with_name(ev_names[i]);
            right_var = right_var ^ temp;
            new_ev    = new_ev.add(temp, evidence[evidence.get_variables()[i]]);
        }
    } else {
        new_ev    = evidence;
        right_var = evidence.get_variables();
    }

    for(auto target : target_variable) {
        if(evidence.get_variables().contains(target)) {
            // If the current target is also an evidence, recreate the table from the plValues
            vector<plProbValue> table;
            unsigned int        value_index;
            target.get_type().get_value_index(value_index, evidence[target]);

            for(unsigned int v = 0; v < target.get_type().cardinality(); v++) {
                table.push_back(v == value_index);
            }

            result *= plDistribution(target, table);
        } else {
            plDistribution P;
            plDistribution PNorm;
            CndP = this->getJointDistribution().ask(target, right_var);
            CndP.instantiate(P, new_ev);
            P.compile(PNorm);

            result *= PNorm;
        }
    }
    return result;
}

plComputableObjectList pm15dBN::query_JD(plVariablesConjunction target_variable) {
    plVariablesConjunction right_var;
    plComputableObjectList result;
    plValues               new_ev;
    plDistribution         P;
    plVariable             temp;

    for(auto target : target_variable) {
        plDistribution PNorm;
        P = this->getJointDistribution().ask(target);
        P.compile(PNorm);

        result *= PNorm;
    }
    return result;
}

void pm15dBN::update(pmBayesianNetwork *bn, plVariablesConjunction vars, plValues evidences, std::string suffix) {
    plError::ignore_this_message(62, true);

    vector<plVariable>           tmpOutgoingInterface;
    vector<plComputableObject *> newComputableObjects;
    plCndDistribution            cnd_distri_instantiate;
    plCndJtDistribution          cnd_distri_JT;
    plCndDistribution            cnd_distri_JD;

    vector<string> parents_names;
    vector<string> parents_t_names;

    plJointDistribution jointDistribution = bn->getJointDistribution();
    plJunctionTree      junctionTree(bn->getJointDistribution());

    // récupération des noeuds de l'évidence dans la distribution du 1.5
    plValues evidences_15;

    for(unsigned int i = 0; i < evidences.size(); i++) {
        std::size_t found = evidences.get_variables()[i].name().rfind("_t");
        if(found == evidences.get_variables()[i].name().length() - 2) {
            std::string bn_evidence_name = evidences.get_variables()[i].name().substr(0, found) + suffix;
            evidences_15                 = evidences_15.add(jointDistribution.get_variables().get_variable_with_name(bn_evidence_name), evidences[i]);
        } else {
            evidences_15 = evidences_15.add(jointDistribution.get_variables().get_variable_with_name(evidences.get_variables()[i].name()), evidences[i]);
        }
    }

    // boucle de remplissage de l'OI
    for(auto var : outgoingInterface) {
        tmpOutgoingInterface.push_back(vars.get_variable_with_name(var.name() + suffix));
    }

    plVariablesConjunction evidence_vars  = evidences_15.get_variables();
    vector<string>         evidence_names = evidence_vars.get_names();

    for(unsigned int i = 0; i < outgoingInterface.size(); i++) {
        plVariablesConjunction c_parents_t;
        plVariablesConjunction c_parents;
        plVariablesConjunction right_vars;
        plVariable             c_variable   = outgoingInterface[i];
        plVariable             c_variable_t = tmpOutgoingInterface[i];
        c_parents                           = this->getParents(this->getNode(c_variable));

        parents_names = c_parents.get_names();

        for(auto parent : c_parents) {
            c_parents_t ^= vars.get_variable_with_name(parent.name() + suffix);
        }
        parents_t_names = c_parents_t.get_names();

        //verification de la présence de l'évidence dans les parent du noeud courant et construction des variables de droite
        for(unsigned int j = 0; j < c_parents_t.size(); j++) {
            if(std::find(evidence_names.begin(), evidence_names.end(), parents_t_names[j]) == evidence_names.end()) {
                right_vars ^= c_parents_t[j];
            }
        }
        right_vars ^= evidence_vars;

        //si le noeud observé fait partie de l'evidence
        if(std::find(evidence_names.begin(), evidence_names.end(), c_variable_t.name()) != evidence_names.end()) {

            //si le noeud n'a pas de parents
            if(c_parents.is_empty()) {
                plDeterministic *distri_ev = new plDeterministic(c_variable, plValues().add(c_variable, evidences_15[c_variable_t]));
                newComputableObjects.push_back(distri_ev);
            }

            //si le noeud a des parents
            else {
                plDistributionTable *distri_ev_parent = new plDistributionTable(c_variable, c_parents);
                distri_ev_parent->push_default(plDeterministic(c_variable, plValues().add(c_variable, evidences_15[c_variable_t])));
                newComputableObjects.push_back(distri_ev_parent);
            }
        }
        //si le noeud observé ne fait pas partie de l'evidence
        else {
            //si le noeud n'a pas de parents ou que ses parents appartiennent à l'évidence
            if(c_parents.is_empty() || right_vars.size() == evidence_vars.size()) {
                //si l'évidence est vide
                if(evidence_vars.size() == 0) {
                    vector<plProbValue>  probValues;
                    plHMM::Observation_t test;
                    plHMM::Observation_t test2;
                    plDistribution       temp_distri_JD = junctionTree.ask(c_variable_t).compile();
                    temp_distri_JD.tabulate(probValues);
                    temp_distri_JD.tabulate(test);
                    temp_distri_JD.compute_expectation(test2);

                    plDistribution *distri = new plDistribution(c_variable, probValues);
                    newComputableObjects.push_back(distri);
                }

                //si l'évidence n'est pas vide
                else {
                    vector<plProbValue> probValues;
                    cnd_distri_JD              = junctionTree.ask(c_variable_t, right_vars);
                    plDistribution temp_distri = cnd_distri_JD.instantiate(evidences_15).compile();
                    temp_distri.tabulate(probValues);

                    plDistribution *distri = new plDistribution(c_variable, probValues);
                    newComputableObjects.push_back(distri);
                }
            }

            //si le noeud a des parents
            else {
                vector<plProbValue> probValues;
                cnd_distri_JD = junctionTree.ask(c_variable_t, right_vars);

                //si l'évidence n'est pas vide
                if(evidence_vars.size() != 0) {
                    cnd_distri_JD.partial_instantiate(cnd_distri_instantiate, evidence_vars, evidences_15);

                    cnd_distri_JD = cnd_distri_instantiate;
                    cnd_distri_JD.tabulate(probValues);
                    plCndDistribution *temp_cnd_distri = new plCndDistribution(c_variable, c_parents, probValues);

                    newComputableObjects.push_back(temp_cnd_distri);
                } else {
                    //si l'évidence est vide
                    cnd_distri_JD.tabulate(probValues);

                    plCndDistribution *temp_cnd_distri = new plCndDistribution(c_variable, c_parents, probValues);
                    newComputableObjects.push_back(temp_cnd_distri);
                }
            }
        }
    }

    //mise à jour du modèle
    for(unsigned int i = 0; i < outgoingInterface.size(); i++) {
        plVariable c_OI_variable = outgoingInterface[i];
        this->setComputableObject(this->getNode(c_OI_variable), newComputableObjects[i]);
    }
}

void pm15dBN::updateGT(plValues evidences) {
    plVariablesConjunction varsGT   = this->getVariables();
    std::string            suffixGT = "_t";
    this->update(this, varsGT, evidences, suffixGT);
    this->setFutur(this->getFutur() + 1);
    this->setPresent(this->getPresent() + 1);
}

void pm15dBN::updateG0(plValues evidences, pmDynamicBayesianNetwork *dBN) {
    plVariablesConjunction varsG0   = dBN->getVariablesforG0();
    std::string            suffixG0 = "_t0";
    this->update(&(dBN->getInitial_G()), varsG0, evidences, suffixG0);
}

vector<plComputableObjectList> pm15dBN::predict(vector<plVariablesConjunction> nodes, vector<int> instants, vector<plValues> evidences) {
    plError::ignore_this_message(62, true);

    plValues                       evt;
    plComputableObjectList         results_predict_instant;
    vector<plComputableObjectList> results_predict_all;

    if(PILGRIM_DEBUG)
        std::cout << PILGRIM_PM15DBN_LOG_PREFIX << "Start 1.5 dBN prediction (predict() function)" << std::endl;

    pm15dBN *copy_15dBN = new pm15dBN(this);
    unsigned int      present    = 0;

    for(unsigned int i = 0; i < instants.size(); present++) {
        if(find(instants.begin(), instants.end(), present + 1) != instants.end()) {
            results_predict_instant.clear();

            if(present < evidences.size() && evidences[present].size() > 0) {
                if(this->use_JT) {
                    results_predict_instant = copy_15dBN->query_JT(nodes[i], evidences[present]);
                } else {
                    results_predict_instant = copy_15dBN->query_JD(nodes[i], evidences[present]);
                }
                copy_15dBN->updateGT(evidences[present]);
            } else {
                if(this->use_JT) {
                    results_predict_instant = copy_15dBN->query_JT(nodes[i]);
                } else {
                    results_predict_instant = copy_15dBN->query_JD(nodes[i]);
                }
                copy_15dBN->updateGT(evt);
            }

            i++;
            results_predict_all.push_back(results_predict_instant);
        } else {
            if(present < evidences.size()) {
                copy_15dBN->updateGT(evidences[present]);
            } else {
                copy_15dBN->updateGT(plValues());
            }
        }
    }
    return results_predict_all;
}

vector<vector<int>> pm15dBN::sample(int t_max, plDistribution G0) {
    plVariablesConjunction searched = this->getVariables();
    plVariablesConjunction known;
    plVariablesConjunction allVar = this->getVariables();
    vector<int>            position;
    vector<int>            results;
    vector<vector<int>>    all_results;
    for(auto var : allVar) {
        string tempName = var.name();
        if(0 != tempName.compare(tempName.length() - 2, 2, "_t")) {
            known ^= var;
            searched.remove(var.name());
        }
    }

    plCndDistribution cnd_distri = this->getJointDistribution().ask(searched, known);
    plDistribution    distri     = this->getJointDistribution().ask(known);
    plValues          values     = distri.draw();

    vector<string> known_var_names = values.get_variables().get_names();
    for(unsigned int i = 0; i < searched.size(); i++) {
        for(unsigned int j = 0; j < known.size(); j++) {
            if(0 == known_var_names[j].compare(searched[i].name().substr(0, searched[i].name().size() - 2))) {
                position.push_back(i);
                position.push_back(j);
            }
        }
    }

    G0.draw(results);
    for(unsigned int j = 0; j < position.size(); j += 2) {
        values.set_value(values.get_variables()[position[j + 1]], results[position[j]]);
    }
    all_results.push_back(results);
    results.clear();
    for(int i = 1; i < t_max; i++) {
        distri = cnd_distri.instantiate(values);
        distri.draw(results);
        all_results.push_back(results);
        for(unsigned int j = 0; j < position.size(); j += 2) {
            values.set_value(values.get_variables()[position[j + 1]], results[position[j]]);
        }
        results.clear();
    }
    return all_results;
}

void pm15dBN::generate_csv_samples_Data(const std::string &dataset_filename,
                                        unsigned int       t_max,
                                        int                nbrOfSample,
                                        plDistribution     G0,
                                        bool               num_sequence,
                                        int                num_time_slice) {
    const string file_name    = dataset_filename + ".csv";
    const string file_name_T0 = dataset_filename + "_T0" + ".csv";

    std::ofstream          out(file_name.c_str());
    std::ofstream          out_T0(file_name_T0.c_str());
    vector<string>         printable_names;
    plVariablesConjunction variables_t;

    for(auto var : variables) {
        if(0 == var.name().compare(var.name().length() - 2, 2, "_t")) {
            printable_names.push_back(var.name().substr(0, var.name().size() - 2));
            variables_t ^= var;
        }
    }

    unsigned int num_node = printable_names.size();
    if(num_time_slice > 1) {
        for(int i = 1; i < num_time_slice; i++) {
            for(unsigned int j = 0; j < num_node; ++j) {
                if(i == 1) {
                    printable_names.push_back(printable_names[j] + "_t");
                } else {
                    printable_names.push_back(printable_names[j] + "_t" + "+" + to_string(i - 1));
                }
            }
        }
    }

    // Affichage du numéro de séquence devant chaque ligne
    if(num_sequence) {
        out << "\""
            << "sequence"
            << "\"";
        out_T0 << "\""
               << "sequence"
               << "\"";

        for(unsigned int j = 0; j < printable_names.size(); ++j) {
            // Writes the variable names in the CSV
            out << ";\"" << printable_names[j] << "\"";
            if(j < printable_names.size() / num_time_slice) {
                out_T0 << ";\"" << printable_names[j] << "\"";
            }
        }
        out << std::endl;

        for(int h = 0; h < nbrOfSample; h++) {
            vector<vector<int>>       data = this->sample(t_max, G0);
            std::vector<unsigned int> values(variables_t.dim());

            for(unsigned int i = 0; (i + num_time_slice - 1) < t_max; ++i) {
                out << h + 1;
                if(i == 0) {
                    out_T0 << h + 1;
                }
                // Generates the data and writes it in the .csv
                for(int k = 0; k < num_time_slice; k++) {
                    for(unsigned int j = 0; j < variables_t.size(); ++j) {
                        out << ";" << data[i + k][j];
                        if(i == 0 && k == 0) {
                            out_T0 << ";" << data[i + k][j];
                        }
                    }
                }
                out << std::endl;

                if(i == 0) {
                    out_T0 << std::endl;
                }
            }
        }
    }

    //Pas d'affichage du numéro de séquence
    else {
        out << "\"" << printable_names[0] << "\"";
        out_T0 << "\"" << printable_names[0] << "\"";

        for(unsigned int j = 1; j < printable_names.size(); ++j) {
            // Writes the variable names in the CSV
            out << ";\"" << printable_names[j] << "\"";
            if(j < printable_names.size() / num_time_slice) {
                out_T0 << ";\"" << printable_names[j] << "\"";
            }
        }
        out << std::endl;
        out_T0 << std::endl;

        for(int h = 0; h < nbrOfSample; h++) {
            vector<vector<int>>       data = this->sample(t_max, G0);
            std::vector<unsigned int> values(variables_t.dim());

            for(unsigned int i = 0; (i + num_time_slice - 1) < t_max; ++i) {
                // Generates the data and writes it in the .csv
                out << data[i][0];
                if(i == 0) {
                    out_T0 << data[i][0];
                }
                for(int k = 0; k < num_time_slice; k++) {
                    unsigned int u;
                    (k == 0) ? u = 1 : u = 0;
                    for(; u < variables_t.size(); ++u) {
                        out << ";" << data[i + k][u];
                        if(i == 0 && k == 0) {
                            out_T0 << ";" << data[i + k][u];
                        }
                    }
                }
                out << std::endl;

                if(i == 0) {
                    out_T0 << std::endl;
                }
            }
        }
    }
}
void pm15dBN::set_inference_JT(bool model) {
    this->use_JT = model;
}


pm15dBN::~pm15dBN() {
}
