# -*- coding: utf-8 -*-
"""
data pull script
"""

# imports
from mysql.connector import connect             # import connect module after check
import numpy  as np                             # numpy array manipulation
import pandas as pd                             # We wil be storing our data pulled from DB into dataframes for better management 
from pylab import rcParams                      # I have no idea what this is...
import pkgutil                                  # Just an optional import to check if a library exist in python..not very important
import traceback                                # For tracing errors exception thrown by interpreter

def check_module(module_name, package_name):
    if not pkgutil.find_loader(module_name):              # Check if a module is installed, used to not crash the execution and handle import stuff automatically
        eval("!pip install " + "mysql-connector-python")  # Install the connect module if not by providing the package 'mysql-connector-python' in argument

def get_connection(config):                     # We will pass into this procedure a configuration variable highlighting connection parameters (user, password, host, database, port)
    return connect(**config)                    # Open he connection and connect to the database
def close_connection(connection):               # Close connection after you're done.
    connection.close()

def get_tables_from_db(id_connection, table_config, SQL_query):
    df_tables = dict()
    mysql_connection = get_connection()
    df = dict()                                 # Data import 
    sql_connection = get_connection(db_config)
    

def query_db(query):
    df_table = {}
    mysql_connection = get_connection(db_config)
    with mysql_connection.cursor() as cursor:
        try:
            cursor.execute(query)
            result = cursor.fetchall()
            columns_name = np.array([cursor.description[i][0] for i in range(len(cursor.description)) if cursor.description[i][0]])
            data = []
            for row in result:
                data.append(np.array([row[i] for i in range(len(row))]))
            df_table = pd.DataFrame(data, columns=columns_name)
        except Exception:
            traceback.print_exc()
    return df_table
    
# Requête pour trouver l'ID des participants de la session Quick-Pi entre le 31 Mai et le 1er Juin
def getParticipantMay():
    query="SELECT participant.id, participant.URI as participant_URI, participer.URI as participer_URI, timestamp_server as timestamp_MAI FROM participer, participant, activite, connexion WHERE participant.id=participer.id AND participer.id=activite.id AND timestamp REGEXP \"2022-05-31\" ORDER BY timestamp DESC"
    return query_db(query)

def getParticipantJune():
    query="SELECT participant.id, participant.URI as participant_URI, participer.URI as participer_URI, timestamp_server as timestamp_JUIN FROM participer, participant, activite, connexion WHERE participant.id=participer.id AND participer.id=activite.id AND timestamp REGEXP \"2022-06-03\" ORDER BY timestamp DESC"
    return query_db(query)

def getParticipantMyself():
    query="SELECT participant.id, participant.URI as participant_URI, participer.URI as participer_URI, timestamp_server FROM participer, participant, activite, connexion WHERE participant.id=participer.id AND participer.id=activite.id AND timestamp REGEXP \"2022-07-09\" ORDER BY timestamp DESC"
    return query_db(query)
        
# in this script, we will pulling all the necessesary data that will be used into the contruction of our indicators
# The main will start the data extraction procedure
if __name__ == '__main__':                      # ------------------------------------MAIN--------------------------------------------- #
    # check_module('connect','mysql-connector-python')
    
    ''' (1) if you wish to connect to the SRL database, enter your 'user' and 'password' in the _db_config variable below this comment. 
    If you do not have those any user oru password, contact <mathias.hiron@gmail.com> for gaining access '''
    db_config = {
        'user'    : 'amine',
        'password': 'n1hFerXeQa2MRh7e',
        'host'    : 'franceioi.cinniket56wn.eu-central-1.rds.amazonaws.com',
        'database': 'srl',
        'port'    : '3306'
    }
    
    # if you wish to select tables you're interested in, uncomment what you need and comment what you don't need
    tables_config    = [
        # "participant", # This table lists all participants 
        # "participer",  # This table lists all participants who have participated into an activityvb b
        # "activite",    # This table lists all activities of a participant 
        "clavier",       # This table lists all keyboard logs, mainly used to track participant's inputs to check if user is idle or not
        # "souris",      # This table lists all mouse logs, mainly used to track participant's mouse position to check if it has changed between two timestamp (Idle check)
        "modification",  # This table tracks all modifications that has been made by a participant in the code editor
        "navigation",    # This table tracks the participant's navigation through the platform (whether they are on the Help/Exercice/Home screen)
        "pas_a_pas",     # This table tracks the participant's usage of the step component in the platform
        "validation",    # This table 
        "connexion",
        # "sujet"
        ]

    # MMerge dataframes using participant URI
    # res = pd.merge(getParticipantMay(), getParticipantJune(), how='inner', on=['participant_URI'])
    res = getParticipantMyself()
    # Data export to be analyzed/printed
    res.to_excel(r'C:\Users\admin\Desktop\Quick-Pi Experiments\Data Analysis\table-participant\participant-code.xlsx', index=False, header=True) 
    print(res)
    # make a query to select all validation errors of students at 31-05-2022  
    

    
    

    
    
    
    
            
    

    



