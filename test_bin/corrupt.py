import sys

def corrupt_file(filename, offset, new_bytes):
    with open(filename, 'r+b') as f:
        f.seek(offset)
        f.write(new_bytes)

# Utilisation : python3 corrupt.py <fichier>
if len(sys.argv) == 2:
    file = sys.argv[1]
    # Exemple 1 : Détruire le magic number (offset 0)
    # corrupt_file(file, 0, b'\x7fXLF') 
    
    # Exemple 2 : Rendre e_shoff (offset 40 pour 64-bits) énorme
    corrupt_file(file, 40, b'\xff\xff\xff\xff\xff\xff\xff\xff')
    
    # Exemple 3 : Mettre e_shnum (offset 60) à 0 ou à max
    # corrupt_file(file, 60, b'\xff\xff')
    
    print(f"{file} a été corrompu avec succès !")
