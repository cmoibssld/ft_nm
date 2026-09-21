import sys
import struct

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 corrupter.py <binaire_elf_64_valide>")
        sys.exit(1)

    filename = sys.argv[1]
    with open(filename, 'rb') as f:
        original_data = bytearray(f.read())

    # Vérification basique (Magic numbers ELF et classe 64-bits)
    if original_data[:4] != b'\x7fELF' or original_data[4] != 2:
        print("Erreur: Veuillez fournir un binaire ELF 64-bits valide.")
        sys.exit(1)

    print(f"Analyse de {filename}...")

    # --- TEST 1 : Troncature brute (fichier coupé) ---
    # Simule un fichier où la mémoire s'arrête avant la fin des en-têtes
    with open("crash_1_truncated.o", "wb") as f:
        f.write(original_data[:len(original_data) // 2])
    print("[+] crash_1_truncated.o généré (Fichier tronqué en plein milieu).")


    # --- TEST 2 : st_name pointe hors limites ---
    data_huge_offset = bytearray(original_data)
    # Parse l'en-tête ELF64 pour trouver la table des sections
    e_shoff, = struct.unpack_from('<Q', data_huge_offset, 0x28)
    e_shentsize, e_shnum, _ = struct.unpack_from('<HHH', data_huge_offset, 0x3A)

    symtab_found = False
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_type, = struct.unpack_from('<I', data_huge_offset, sh_offset + 0x04)
        
        if sh_type == 2: # 2 = SHT_SYMTAB (Table des symboles)
            sym_offset, = struct.unpack_from('<Q', data_huge_offset, sh_offset + 0x18)
            sym_size, = struct.unpack_from('<Q', data_huge_offset, sh_offset + 0x20)
            
            if sym_size > 24: # S'il y a plus que le symbole NUL (index 0)
                # On modifie le st_name du premier symbole valide (index 1, à l'offset 24)
                # st_name est un entier 32 bits (4 octets)
                struct.pack_into('<I', data_huge_offset, sym_offset + 24, 0x7FFFFFFF)
                symtab_found = True
            break

    if symtab_found:
        with open("crash_2_huge_offset.o", "wb") as f:
            f.write(data_huge_offset)
        print("[+] crash_2_huge_offset.o généré (Index de nom de symbole pointant à 2Go).")
    else:
        print("[-] Impossible de trouver .symtab. Compile ton binaire avec 'gcc -g'.")


    # --- TEST 3 : Absence de \0 dans la table des chaînes ---
    data_no_null = bytearray(original_data)
    # On remplace tous les octets nuls (\0) des 150 derniers octets par 'A' (0x41)
    # Dans la plupart des petits binaires, les sections .strtab et .shstrtab sont à la fin.
    for i in range(len(data_no_null) - 150, len(data_no_null)):
        if data_no_null[i] == 0x00:
            data_no_null[i] = 0x41

    with open("crash_3_no_null.o", "wb") as f:
        f.write(data_no_null)
    print("[+] crash_3_no_null.o généré (Suppression des '\\0' de fin de fichier).")

if __name__ == '__main__':
    main()