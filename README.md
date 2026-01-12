
Repozitár zabezpečuje distribuovanie učebných materiálov, zadaní, prednášok k cvičeniam a zdrojových kódov študentom RRM LS 2025/2026.

# Ako sa dostať ku konkretnému cvičeniu?
Predpokladajme že je prvý týždeň a chceme sa dostať k zadaniu z cvičenia 1.

Možnosti sú:
- git CLI
- gitkraken

### Git CLI
V termináli sa dostante do priečinka, kam chcete repozitár so zadaniami skopírovať. Následne
```bash
git clone https://github.com/Jakub-Ivan-STUBA/rrm-zadania.git
```
<span style="color: orange">Clonovanie repa je jednorázova akcia, stačí ju spraviť raz!</span>

Potom si k sebe stiahneme najnovšiu verziu repozitára 
```bash
git pull
```

Vyberieme branchu, na ktorej je zadanie_1 - `cvicenie_1`
```bash
git checkout cvicenie_1
```

### GitKraken
Nainštalujte si GitKraken, využite študentskú licenciu (ISIC). Následne si naclonujte repozitár v novom tabe a prekliknite sa na branchu `cvicenie_1`.
