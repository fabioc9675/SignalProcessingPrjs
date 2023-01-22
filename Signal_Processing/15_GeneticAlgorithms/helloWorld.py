import numpy as np
import random

geneSet = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 '


class DNA:
    def __init__(self, target, mutation_rate, n_individuals, n_selection, n_generations, verbose=True):
        self.target = target
        self.mutation_rate = mutation_rate
        self.n_individuals = n_individuals
        self.n_selection = n_selection
        self.n_generations = n_generations
        self.verbose = verbose

    def create_individual(self, min=0, max=len(geneSet)):
        '''
        Crea un individuo
        '''
        individual = [geneSet[np.random.randint(min, max)]
                      for i in range(len(self.target))]
        return(individual)

    def create_population(self):
        '''
        Crea una poblacion
        '''
        population = [self.create_individual()
                      for i in range(self.n_individuals)]
        return(population)

    def fitness(self, individual):
        '''
        Evaluar el individuo, regresa el numero de letras que son iguales
        '''
        fitness = 0
        for i in range(len(individual)):
            if individual[i] == self.target[i]:
                fitness += 1
        return(fitness)

    def selection(self, population):
        '''
        Realiza la seleccion de los mejores individuos evaluando el score del fitness
        '''
        scores = [(self.fitness(i), i) for i in population]
        # Seleccion de los mejores adaptados, es decir, los que mas datos iguales tienen
        scores = [i[1] for i in sorted(scores)]

        # seleccion por competicion, seleccion de los mejores
        selected = scores[len(scores) - self.n_selection:]
        return(selected)

    def reproduction(self, population, selected):
        '''
        Funcion de reproduccion, produce nuevos datos a partir de dos datos existentes en la anterior generacion
        '''
        point = 0
        father = []

        for i in range(len(population)):
            # reemplazaremos los valores en los hijos
            point = np.random.randint(1, len(self.target) - 1)
            father = random.sample(selected, 2)

            population[i][:point] = father[0][:point]
            population[i][point:] = father[1][point:]

        return(population)

    def mutation(self, population):
        '''
        Mutaremos la poblacion ya reproducida
        '''
        for i in range(len(population)):
            if random.random() <= self.mutation_rate:
                point = random.randint(0, len(self.target)-1)
                new_value = geneSet[np.random.randint(0, len(geneSet))]

                while new_value == population[i][point]:
                    new_value = geneSet[np.random.randint(0, len(geneSet))]

                population[i][point] = new_value

        return(population)

    def run_geneticalgo(self):

        population = self.create_population()
        # el fitness tambien puede ser un criterio de parada
        for i in range(self.n_generations):
            maxFitness = 0
            finish = False

            if self.verbose:
                print('___________')
                print('Generacion: ', i)
                print('Poblacion', population)

            for j in range(len(population)):
                fitness = self.fitness(population[j])

                if fitness > maxFitness:
                    maxFitness = fitness

                if(fitness == len(self.target)):
                    print("Cromosoma encontrado en la posicion " +
                          str(j), 'cromosoma = ', population[j])
                    finish = True

            if self.verbose:
                print("max Fitness = ", maxFitness)
                print()

            if finish == True:
                return

            # seleccion de individuos
            selected = self.selection(population)
            population = self.reproduction(population, selected)
            population = self.mutation(population)


def main():
    target = "Hola Margarita"  # cromosoma al que queremos llegar
    model = DNA(target=target, mutation_rate=0.1, n_individuals=50,
                n_selection=15, n_generations=2000, verbose=True)
    model.run_geneticalgo()


if __name__ == "__main__":
    main()
