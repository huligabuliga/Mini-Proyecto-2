void selectTableOne(struct alumno *alumnos, int cont, int columnas[3], int opp, int col, int valNum, char valStr[150], char string[20000])
{
    for (int i = 0; i < cont; i++){
        char temp[100];

        switch (col){
        case 1: // matricula
            if (opp == 1){
                if (alumnos[i].matricula != valNum)
                    continue;
            }
            else if (opp == 2){
                if (alumnos[i].matricula == valNum)
                    continue;
            }
            else if (opp == 3){
                if (alumnos[i].matricula > valNum)
                    continue;
            }
            else if (opp == 4){
                if (alumnos[i].matricula < valNum)
                    continue;
            }
            break;
        case 2: // nombre
            if (opp == 1){
                if (strcasecmp(alumnos[i].nombre, valStr) != 0)
                    continue;
            }
            else if (opp == 2){
                if (strcasecmp(alumnos[i].nombre, valStr) == 0)
                    continue;
            }
            break;
        case 3: // apellido
            if (opp == 1){
                if (strcasecmp(alumnos[i].apellido, valStr) != 0)
                    continue;
            }
            else if (opp == 2){
                if (strcasecmp(alumnos[i].apellido, valStr) == 0)
                    continue;
            }
            break;

        default:
            break;
        }
        if (columnas[0] == 1){
            snprintf(temp, sizeof temp, "%d;", alumnos[i].matricula);
            strcat(string, temp);
        }
        if (columnas[1] == 1){
            snprintf(temp, sizeof temp, "%s;", alumnos[i].nombre);
            strcat(string, temp);
        }
        if (columnas[2] == 1){
            snprintf(temp, sizeof temp, "%s;", alumnos[i].apellido);
            strcat(string, temp);
        }
        
        strcat(string, "\n");
    }
};

void selectTableTwo(struct carrera *carreras, int cont, int columnas[3], int opp, int col, int valNum, char valStr[150], char string[20000])
{
    for (int i = 0; i < cont; i++){
        char temp[500];

        switch (col){
        case 1: // carreraId
            if (opp == 1){
                if (carreras[i].carreraId != valNum)
                    continue;
            }
            else if (opp == 2){
                if (carreras[i].carreraId == valNum)
                    continue;
            }
            else if (opp == 3){
                if (carreras[i].carreraId > valNum)
                    continue;
            }
            else if (opp == 4){
                if (carreras[i].carreraId < valNum)
                    continue;
            }
            break;
        case 2: // semestre
            if (opp == 1){
                if (carreras[i].smestre != valNum)
                    continue;
            }
            else if (opp == 2){
                if (carreras[i].smestre == valNum)
                    continue;
            }
            else if (opp == 3){
                if (carreras[i].smestre > valNum)
                    continue;
            }
            else if (opp == 4){
                if (carreras[i].smestre < valNum)
                    continue;
            }
            break;
        case 3: // promedio
            if (opp == 1){
                if (carreras[i].promedio != valNum)
                    continue;
            }
            else if (opp == 2){
                if (carreras[i].promedio == valNum)
                    continue;
            }
            else if (opp == 3){
                if (carreras[i].promedio > valNum)
                    continue;
            }
            else if (opp == 4){
                if (carreras[i].promedio < valNum)
                    continue;
            }
            break;

        default:
            break;
        }
        if (columnas[0] == 1){
            snprintf(temp, sizeof temp, "%s;", carreras[i].carreraId);
            strcat(string, temp);
        }
        if (columnas[1] == 1){
            snprintf(temp, sizeof temp, "%d;", carreras[i].semestre);
            strcat(string, temp);
        }
        if (columnas[2] == 1){
            snprintf(temp, sizeof temp, "%0.f;", carreras[i].promedio);
            strcat(string, temp);
        }
        strcat(string, "\n");
    }
};

int joinCondition(struct alumno alumnoJ, int col1, struct carrera carreraJ, int col2){
    switch (col1){
    case 1:            
        if(col2 == 1){
            return(alumnoJ.matricula == carreraJ.carreraId);
        }
    break;
    case 2:
        if(col2 == 1){
            return(alumnoJ.matricula == carreraJ.semestre);
        }
    break;
     case 3:
        if(col2 == 1){
            return(alumnoJ.matricula == carreraJ.promedio);
        }
    break;
     case 4:
        if(col2 == 1){
            return(alumnoJ.nombre == carreraJ.carreraId);
        }
    break;
    case 5:
        if(col2 == 1){
            return(alumnoJ.nombre == carreraJ.semestre);
        }
    break;
    case 6:
        if(col2 == 1){
            return(alumnoJ.nombre == carreraJ.promedio);
        }
    break;
    case 7:
        if(col2 == 1){
            return(alumnoJ.apellido == carreraJ.carreraId);
        }
    break;
    case 8:
        if(col2 == 1){
            return(alumnoJ.apellido == carreraJ.semestre);
        }
    break;
    case 9:
        if(col2 == 1){
            return(alumnoJ.apellido == carreraJ.promedio);
        }
    break;
    default:
        return 0;
        break;
    }

    return 0;
}

void joinTable(struct alumno *alumnos, int contA, int columnas1[9], struct carrera *carreras, int contC, int columnas2[5], int col1, int col2, char string[20000]){
    char temp[500];

    for (int i = 0; i < contA; i++){
        for (int j = 0; j < contC; j++){
            if (joinCondition(alumnos[i], col1, carreras[j], col2) == 1){
                if (columnas1[0] == 1){
                    snprintf(temp, sizeof temp, "%d;", alumnos[i].matricula);
                    strcat(string, temp);
                }
                if (columnas1[1] == 1){
                    snprintf(temp, sizeof temp, "%s;", alumnos[i].nombre);
                    strcat(string, temp);
                }
                if (columnas1[2] == 1){
                    snprintf(temp, sizeof temp, "%s;", alumnos[i].apellido);
                    strcat(string, temp);
                }
                if (columnas2[0] == 1){
                    snprintf(temp, sizeof temp, "%s;", carreras[j].carreraId);
                    strcat(string, temp);
                }
                if (columnas2[1] == 1){
                    snprintf(temp, sizeof temp, "%d;", carreras[j].semestre);
                    strcat(string, temp);
                }
                if (columnas2[2] == 1){
                    snprintf(temp, sizeof temp, "%0.f;", carreras[j].promedio);
                    strcat(string, temp);
                }
                strcat(string, "\n");
                break;
            }
        }

    }
}