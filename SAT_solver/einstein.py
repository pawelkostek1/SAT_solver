def permute(numOfItems, set):
    if numOfItems == 1:
        permutation = []
        for element in set:
            permutation.append([element])
        return permutation
    else:
        permutation = []
        permute_recursive = permute(numOfItems-1, set)
        for element in set:
            for perm in permute_recursive:
                if element not in perm:
                    for i in range(len(perm)):
                        perm_copy = perm.copy()
                        perm_copy.insert(i, element)
                        permutation.append(perm_copy)
        return permutation



def main():
    nationality = ["Brit", "Swede", "Dane", "German", "Norwegian"]

    house = ["H1", "H2", "H3", "H4", "H5"]
    house_color = ["Red", "Green", "White", "Yellow", "Blue"]
    pets = ["Dogs", "Cats", "Birds", "Fish", "Horse"]
    drink = ["Tea", "Coffee", "Water", "Milk", "Beer"]
    smoke = ["Bluemasters", "PallMall", "Dunhill", "Prince", "Blends"]

    attributes = [house, house_color, pets, drink, smoke]
    attribute_names = ["house", "house color", "pet", "drink", "cigarette"]

    variables = {}
    i = 1

    for attribute in attributes:

        for nat in nationality:
            for item in attribute:
                variables[nat[0]+"_"+item] = i
                i = i+1
    #Print all the variables
    for variable in variables.keys():
        print(variable + ": " + str(variables[variable]))

    ###########################################
    #Below we generate the riddle encodings
    ###########################################
    f = open('einstein.cnf', 'w')

    print("####################################################")
    print("c This file contains the Einstain's puzzle expressed in CNF")
    f.write("c This file contains the Einstain's puzzle expressed in CNF"+"\n")
    print("p cnf 125 1075")
    f.write("p cnf 125 5"+"\n")

    print("c The Brit live in the red house.")
    #f.write("c The Brit live in the red house."+"\n")
    #print(nationality[0][0]+"_"+house_color[0])
    print(str(variables[nationality[0][0] + "_" + house_color[0]]))
    f.write(str(variables[nationality[0][0] + "_" + house_color[0]])+" 0"+"\n")

    print("c The Swede keeps dogs as pets.")
    #f.write("c The Swede keeps dogs as pets."+"\n")
    #print(nationality[1][0]+"_"+pets[0])
    print(str(variables[nationality[1][0] + "_" + pets[0]]))
    f.write(str(variables[nationality[1][0] + "_" + pets[0]])+" 0"+"\n")

    print("c The Dane drinks tea.")
    #f.write("c The Dane drinks tea."+"\n")
    #print(nationality[2][0]+"_"+drink[0])
    print(str(variables[nationality[2][0] + "_" + drink[0]]))
    f.write(str(variables[nationality[2][0] + "_" + drink[0]])+" 0"+"\n")

    print("c The German smokes Prince.")
    #f.write("c The German smokes Prince."+"\n")
    #print(nationality[3][0]+"_"+smoke[3])
    print(str(variables[nationality[3][0] + "_" + smoke[3]]))
    f.write(str(variables[nationality[3][0] + "_" + smoke[3]])+" 0"+"\n")

    print("c The Norwegian lives in the first house.")
    #f.write("c The Norwegian lives in the first house."+"\n")
    #print(nationality[4][0]+"_"+house[0])
    print(str(variables[nationality[4][0] + "_" + house[0]]))
    f.write(str(variables[nationality[4][0] + "_" + house[0]])+" 0"+"\n")

    print("c The green house's owner drinks coffee.")
    #f.write("c The green house's owner drinks coffee."+"\n")
    for nat in nationality:
        #print("-"+nat[0]+"_Green"+" "+nat[0]+"_Coffee")
        print(str(-variables[nat[0] + "_Green"]) + " " + str(variables[nat[0] + "_Coffee"]))
        f.write(str(-variables[nat[0] + "_Green"]) + " " + str(variables[nat[0] + "_Coffee"])+" 0"+"\n")

    print("c The person who smokes PallMall rears birds.")
    #f.write("c The person who smokes PallMall rears birds."+"\n")
    for nat in nationality:
        #print("-"+nat[0]+"_PallMall"+" "+nat[0]+"_Birds")
        print(str(-variables[nat[0] + "_PallMall"]) + " " + str(variables[nat[0] + "_Birds"]))
        f.write(str(-variables[nat[0] + "_PallMall"]) + " " + str(variables[nat[0] + "_Birds"])+" 0"+"\n")

    print("c The owner of the yellow house smokes Dunhill.")
    #f.write("c The owner of the yellow house smokes Dunhill."+"\n")
    for nat in nationality:
        #print("-"+nat[0]+"_Yellow"+" "+nat[0]+"_Dunhill")
        print(str(-variables[nat[0] + "_Yellow"]) + " " + str(variables[nat[0] + "_Dunhill"]))
        f.write(str(-variables[nat[0] + "_Yellow"]) + " " + str(variables[nat[0] + "_Dunhill"])+" 0"+"\n")

    print("c The man living in the center house drinks milk.")
    #f.write("c The man living in the center house drinks milk."+"\n")
    for nat in nationality:
        #print("-"+nat[0]+"_H3"+" "+nat[0]+"_Milk")
        print(str(-variables[nat[0] + "_H3"]) + " " + str(variables[nat[0] + "_Milk"]))
        f.write(str(-variables[nat[0] + "_H3"]) + " " + str(variables[nat[0] + "_Milk"])+" 0"+"\n")

    print("c The owner who smokes Bluemasters drinks beer.")
    #f.write("c The owner who smokes Bluemasters drinks beer."+"\n")
    for nat in nationality:
        #print("-"+nat[0]+"_Bluemasters"+" "+nat[0]+"_Beer")
        print(str(-variables[nat[0] + "_Bluemasters"]) + " " + str(variables[nat[0] + "_Beer"]))
        f.write(str(-variables[nat[0] + "_Bluemasters"]) + " " + str(variables[nat[0] + "_Beer"])+" 0"+"\n")


    print("c The green house is on the left of the white house.")
    #f.write("c The green house is on the left of the white house."+"\n")
    national_permute = permute(2,nationality)
    for nat_permute in national_permute:
        for i in range(1,5):
            #print("-"+nat_permute[0][0] + "_" + "H" + str(i) + " " +
            #    "-"+nat_permute[0][0] + "_" + "Green" + " " +
            #    "-"+nat_permute[1][0] + "_" + "H" + str(i + 1)+ " " +
            #       nat_permute[1][0] + "_" + "White")
            print(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Green"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "White"]))
            f.write(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Green"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "White"])+" 0"+"\n")

    print("c The man who smokes Blends lives next to the one who keeps cats.")
    #f.write("c The man who smokes Blends lives next to the one who keeps cats."+"\n")
    for nat_permute in national_permute:
        for i in range(1,5):
            #print("-"+nat_permute[0][0] + "_" + "H" + str(i) + " " +
            #    "-"+nat_permute[0][0] + "_" + "Blends" + " " +
            #    "-"+nat_permute[1][0] + "_" + "H" + str(i + 1)+ " " +
            #       nat_permute[1][0] + "_" + "Cats")
            print(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Cats"]))
            f.write(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Cats"])+" 0"+"\n")
        for i in range(5,1, -1):
            #print("-"+nat_permute[0][0] + "_" + "H" + str(i) + " " +
            #    "-"+nat_permute[0][0] + "_" + "Blends" + " " +
            #    "-"+nat_permute[1][0] + "_" + "H" + str(i - 1)+ " " +
            #       nat_permute[1][0] + "_" + "Cats")
            print(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i - 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Cats"]))
            f.write(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i - 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Cats"])+" 0"+"\n")


    print("c The man who who keeps the horse lives next to the man who smokes Dunhill.")
    #f.write("c The man who who keeps the horse lives next to the man who smokes Dunhill."+"\n")
    for nat_permute in national_permute:
        for i in range(1,5):
            #print("-"+nat_permute[0][0] + "_" + "H" + str(i) + " " +
            #    "-"+nat_permute[0][0] + "_" + "Horse" + " " +
            #    "-"+nat_permute[1][0] + "_" + "H" + str(i + 1)+ " " + nat_permute[1][0] + "_" + "Dunhill")
            print(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Horse"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Dunhill"]))
            f.write(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Horse"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i + 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Dunhill"])+" 0"+"\n")
        for i in range(5,1, -1):
            #print("-"+nat_permute[0][0] + "_" + "H" + str(i) + " " +
            #    "-"+nat_permute[0][0] + "_" + "Horse" + " " +
            #    "-"+nat_permute[1][0] + "_" + "H" + str(i - 1)+ " " + nat_permute[1][0] + "_" + "Dunhill")
            print(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Horse"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i - 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Dunhill"]))
            f.write(str(-variables[nat_permute[0][0] + "_" + "H" + str(i)]) + " " +
                  str(-variables[nat_permute[0][0] + "_" + "Horse"]) + " " +
                  str(-variables[nat_permute[1][0] + "_" + "H" + str(i - 1)]) + " " +
                  str(variables[nat_permute[1][0] + "_" + "Dunhill"])+" 0"+"\n")

    print("c The Norwegian lives next to the blue house.")
    #f.write("c The Norwegian lives next to the blue house."+"\n")
    for nat in nationality:
        if nat != "Norwegian":
            #print("-"+nat[0] + "_" + "H1" + " " +
            #    "-"+nat[0] + "_" + "Blue" + " " +
            #        "N" + "_" + "H2")
            print(str(-variables[nat[0] + "_" + "H1"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H2"]))
            f.write(str(-variables[nat[0] + "_" + "H1"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H2"])+" 0"+"\n")

            #print("-"+nat[0] + "_" + "H2" + " " +
            #    "-"+nat[0] + "_" + "Blue" + " " +
            #    "N" + "_" + "H1" + " " + "N" + "_" + "H3")
            print(str(-variables[nat[0] + "_" + "H2"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H1"]) + " " +
                  str(variables["N" + "_" + "H3"]))
            f.write(str(-variables[nat[0] + "_" + "H2"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H1"]) + " " +
                  str(variables["N" + "_" + "H3"])+" 0"+"\n")

            #print("-"+nat[0] + "_" + "H3" + " " +
            #    "-"+nat[0] + "_" + "Blue" + " " +
            #    "N" + "_" + "H2" + " " + "N" + "_" + "H4")
            print(str(-variables[nat[0] + "_" + "H3"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H2"]) + " " +
                  str(variables["N" + "_" + "H4"]))
            f.write(str(-variables[nat[0] + "_" + "H3"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H2"]) + " " +
                  str(variables["N" + "_" + "H4"])+" 0"+"\n")

            #print("-"+nat[0] + "_" + "H4" + " " +
            #    "-"+nat[0] + "_" + "Blue" + " " +
            #    "N" + "_" + "H3" + " " + "N" + "_" + "H5")
            print(str(-variables[nat[0] + "_" + "H4"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H3"]) + " " +
                  str(variables["N" + "_" + "H5"]))
            f.write(str(-variables[nat[0] + "_" + "H4"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H3"]) + " " +
                  str(variables["N" + "_" + "H5"])+" 0"+"\n")

            #print("-"+nat[0] + "_" + "H5" + " " +
            #    "-"+nat[0] + "_" + "Blue" + " " +
            #    "N" + "_" + "H4")
            print(str(-variables[nat[0] + "_" + "H5"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H4"]))
            f.write(str(-variables[nat[0] + "_" + "H5"]) + " " +
                  str(-variables[nat[0] + "_" + "Blue"]) + " " +
                  str(variables["N" + "_" + "H4"])+" 0"+"\n")

    print("c The man who smokes Blends has a neighbor who drinks water.")
    #f.write("c The man who smokes Blends has a neighbor who drinks water."+"\n")
    for nat_permute in national_permute:
        #print("-" + nat_permute[0][0] + "_" + "H1" + " " +
        #      "-" + nat_permute[0][0] + "_" + "Blends" + " " +
        #      "-" + nat_permute[1][0] + "_" + "Water" + " " +
        #            nat_permute[1][0] + "_" + "H2")
        print(str(-variables[nat_permute[0][0] + "_" + "H1"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H2"]))
        f.write(str(-variables[nat_permute[0][0] + "_" + "H1"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H2"])+" 0"+"\n")

        #print("-" + nat_permute[0][0] + "_" + "H2" + " " +
        #      "-" + nat_permute[0][0] + "_" + "Blends" + " " +
        #      "-" + nat_permute[1][0] + "_" + "Water" + " " +
        #            nat_permute[1][0] + "_" + "H1" + " " +
        #            nat_permute[1][0] + "_" + "H3")
        print(str(-variables[nat_permute[0][0] + "_" + "H2"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H1"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H3"]))
        f.write(str(-variables[nat_permute[0][0] + "_" + "H2"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H1"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H3"])+" 0"+"\n")

        #print("-" + nat_permute[0][0] + "_" + "H3" + " " +
        #      "-" + nat_permute[0][0] + "_" + "Blends" + " " +
        #      "-" + nat_permute[1][0] + "_" + "Water" + " " +
        #            nat_permute[1][0] + "_" + "H2" + " " +
        #            nat_permute[1][0] + "_" + "H4")
        print(str(-variables[nat_permute[0][0] + "_" + "H3"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H2"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H4"]))
        f.write(str(-variables[nat_permute[0][0] + "_" + "H3"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H2"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H4"])+" 0"+"\n")

        #print("-" + nat_permute[0][0] + "_" + "H4" + " " +
        #      "-" + nat_permute[0][0] + "_" + "Blends" + " " +
        #      "-" + nat_permute[1][0] + "_" + "Water" + " " +
        #            nat_permute[1][0] + "_" + "H3" + " " +
        #            nat_permute[1][0] + "_" + "H5")
        print(str(-variables[nat_permute[0][0] + "_" + "H4"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H3"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H5"]))
        f.write(str(-variables[nat_permute[0][0] + "_" + "H4"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H3"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H5"])+" 0"+"\n")

        #print("-" + nat_permute[0][0] + "_" + "H5" + " " +
        #      "-" + nat_permute[0][0] + "_" + "Blends" + " " +
        #      "-" + nat_permute[1][0] + "_" + "Water" + " " +
        #            nat_permute[1][0] + "_" + "H4")
        print(str(-variables[nat_permute[0][0] + "_" + "H5"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H4"]))
        f.write(str(-variables[nat_permute[0][0] + "_" + "H5"]) + " " +
              str(-variables[nat_permute[0][0] + "_" + "Blends"]) + " " +
              str(-variables[nat_permute[1][0] + "_" + "Water"]) + " " +
              str(variables[nat_permute[1][0] + "_" + "H4"])+" 0"+"\n")

    for k in range(len(attributes)):
        print("c Persons have different "+attribute_names[k]+"s.")
        #f.write("c Persons have different "+attribute_names[k]+"s."+"\n")
        print("c For each nationality there exists at least one "+attribute_names[k]+".")
        #f.write("c For each nationality there exists at least one "+attribute_names[k]+"."+"\n")
        for nat in nationality:
            #print(nat[0] +"_"+attributes[k][0]+" "+
            #      nat[0] +"_"+attributes[k][1]+" "+
            #      nat[0] +"_"+attributes[k][2]+" "+
            #      nat[0] +"_"+attributes[k][3]+" "+
            #      nat[0] +"_"+attributes[k][4])
            print(str(variables[nat[0] +"_"+attributes[k][0]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][1]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][2]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][3]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][4]]))
            f.write(str(variables[nat[0] +"_"+attributes[k][0]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][1]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][2]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][3]])+" "+
                  str(variables[nat[0] +"_"+attributes[k][4]])+" 0"+"\n")

        print("c For each nationality there exists at most one "+attribute_names[k]+".")
        #f.write("c For each nationality there exists at most one "+attribute_names[k]+"."+"\n")
        for nat in nationality:
            for i in range(len(attributes[k])):
                for j in range(i+1,len(attributes[k])):
                    #print("-"+nat[0]+"_"+attributes[k][i]+" "+
                    #      "-"+nat[0]+"_"+attributes[k][j])
                    print(str(-variables[nat[0]+"_"+attributes[k][i]])+" "+
                          str(-variables[nat[0]+"_"+attributes[k][j]]))
                    f.write(str(-variables[nat[0]+"_"+attributes[k][i]])+" "+
                          str(-variables[nat[0]+"_"+attributes[k][j]])+" 0"+"\n")

        print("c Each "+attribute_names[k]+" cannot be associated with two (or more) persons.")
        #f.write("c Each "+attribute_names[k]+" cannot be associated with two (or more) persons."+"\n")
        for a in attributes[k]:
            for i in range(len(nationality)):
                for j in range(i + 1, len(nationality)):
                    #print("-" + nationality[i][0] + "_" + a + " " +
                    #      "-" + nationality[j][0] + "_" + a)
                    print(str(-variables[nationality[i][0] + "_" + a]) + " " +
                          str(-variables[nationality[j][0] + "_" + a]))
                    f.write(str(-variables[nationality[i][0] + "_" + a]) + " " +
                          str(-variables[nationality[j][0] + "_" + a])+" 0"+"\n")
    f.close()
if __name__ == '__main__':
    main()
