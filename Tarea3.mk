CFLAGS= -g -Wall -Wextra -lm

Resultados_hw3.pdf : Resultados_hw3.tex
	
Resultados_hw3.tex : Onda.png orbitas.png
		pdflatex Resultados_hw3.tex	

Onda.png : Onda.py
		python Onda.py
		
orbitas.png: Plots_Planetas.py
	python Plots_Planetas.py
		
Plots_Planetas.py : resultadosx.csv
	python Plots_Planetas.py
	
resultadosx.csv: Planetas.x
	./Planetas.x
	
Planetas.x: Planetas.c
	cc Planetas.c $(CFLAGS) -o Planetas.x
	
clean:
	rm -f *.aux *.log masas.csv
		echo Clean done

