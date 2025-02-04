#pragma once

#include "erloadout_messages.hpp"

using namespace std;
using namespace erloadout;

msg::LoadoutMessages msg::loadout_messages;

/**
 * Map of Steam languages to messages.
 *
 * Currently, only English is fully localized. Some messages are available in other languages
 * because they're duplicates of strings in the vanilla game, strings in other games, or small
 * phrases that can be machine translated. Translation contributions are absolutely welcome.
 */
const map<string, msg::LoadoutMessages> msg::loadout_messages_by_lang = {
    {"english",
     {
         .manage_loadouts = L"Tarnished loadouts",
         .save_loadout = L"Save loadout",
         .apply_loadout = L"Apply loadout",
         .select_loadout_to_save = L"Select a slot to save your current loadout",
         .select_loadout_to_apply = L"Select a loadout to apply",
         .save_loadout_prompt = L"Overwrite [<?itemName?>] with current loadout?",
         .apply_loadout_prompt = L"Apply [<?itemName?>]?",
         .loadout = L"Loadout",
         .empty_slot = L"Empty Slot",
         .armaments = L"Armaments",
         .arrows_bolts = L"Arrows/Bolts",
         .armor = L"Armor",
         .talismans = L"Talismans",
         .unarmed = L"Unarmed",
         .none = L"None",
         .loadout_book = L"Tarnished Combat Manual",
     }},

    // deude / Deutsch (German)
    {"german",
     {
         .manage_loadouts = L"Ausrüstung der Befleckten",
         // Machine translated
         .save_loadout = L"Ausrüstungen speichern",
         // Machine translated
         .apply_loadout = L"Ausrüstungen anwenden",
         // Machine translated
         .select_loadout_to_save =
             L"Wählen Sie einen Slot, um Ihre aktuelle Ausrüstung zu speichern.",
         // Machine translated
         .select_loadout_to_apply = L"Wählen Sie eine Ausrüstung zum Anwenden",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"[<?itemName?>] überschreiben?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"[<?itemName?>] laden?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Ausrüstung",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Leerer Platz",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Waffen",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Pfeile/Bolzen",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Rüstung",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismane",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Unbewaffnet",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Keine",
         // Manually translated
         .loadout_book = L"Fechtbuch der Befleckten",
     }},

    // frafr / Français (French)
    {"french",
     {
         .manage_loadouts = L"Inventaires de sans-éclat",
         // Machine translated
         .save_loadout = L"Sauvegarder l'inventaire",
         // Machine translated
         .apply_loadout = L"Appliquer les inventaires",
         // Machine translated
         .select_loadout_to_save =
             L"Sélectionnez un emplacement pour sauvegarder vos inventaires actuels.",
         // Machine translated
         .select_loadout_to_apply = L"Sélectionnez un inventaires à appliquer",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"Écraser [<?itemName?>] ?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"Charger [<?itemName?>] ?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Équipement",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Emplacement vide",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Armes",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Flèches/Carreaux",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Armure",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismans",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Sans arme",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Aucun",
         // Machine translated
         .loadout_book = L"Manuel de combat",
     }},

    // itait / Italiano (Italian)
    {"italian",
     {
         .manage_loadouts = L"Equipaggiamenti di Senzaluce",
         // Machine translated
         .save_loadout = L"Salva equipaggiamenti",
         // Machine translated
         .apply_loadout = L"Applicare l'equipaggiamento",
         // Machine translated
         .select_loadout_to_save = L"Selezionare uno slot per salvare l'equipaggiamento attuale",
         // Machine translated
         .select_loadout_to_apply = L"Selezionare un equipaggiamento da applicare",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"Sovrascrivere [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"Caricare [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Equipaggiamento",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Slot vuoto",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Armamenti",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Frecce/Dardi",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Armature",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismani",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Disarmato",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Nessuno",
         // Machine translated
         .loadout_book = L"Manuale di combattimento",
     }},

    // jpnjp / 日本語 (Japanese)
    {"japanese",
     {
         .manage_loadouts = L"装備",
         // Machine translated
         .save_loadout = L"装備の保存",
         // Machine translated
         .apply_loadout = L"装備する",
         // Machine translated
         .select_loadout_to_save = L"現在の装備を保存するスロットを選択する。",
         // Machine translated
         .select_loadout_to_apply = L"適用する装備の選択",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"〈<?itemName?>〉を上書きします",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"〈<?itemName?>〉をロードします",
         // (TF2) Loadout
         .loadout = L"装備",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"記録可能",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"武器",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"矢／ボルト",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"防具",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"タリスマン",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"素手",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"なし",
         // Machine translated
         .loadout_book = L"戦闘マニュアル",
     }},

    // korkr / 한국어 (Korean)
    {"koreana",
     {
         .manage_loadouts = L"장비 설정",
         // Machine translated
         .save_loadout = L"장비 설정 저장하기",
         // Machine translated
         .apply_loadout = L"장비 설정 적용하기",
         // Machine translated
         .select_loadout_to_save = L"현재 장비 설정을 저장할 슬롯을 선택합니다.",
         // Machine translated
         .select_loadout_to_apply = L"적용할 장비 설정을 선택합니다.",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"〈<?itemName?>〉의 데이터를 덮어쓰시겠습니까?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"〈<?itemName?>〉의 데이터를 불러오시겠습니까?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"장비 설정",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"기록 가능",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"무기",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"화살/볼트",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"방어구",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"탈리스만",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"맨손",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"없음",
         // Machine translated
         .loadout_book = L"전투 매뉴얼",
     }},

    // polpl / Polski (Polish)
    {"polish",
     {
         .manage_loadouts = L"Zmatowione wyposażenie",
         // Machine translated
         .save_loadout = L"Zapisywanie wyposażenia",
         // Machine translated
         .apply_loadout = L"Zastosuj wyposażenie",
         // Machine translated
         .select_loadout_to_save = L"Wybierz slot, aby zapisać bieżące wyposażenie.",
         // Machine translated
         .select_loadout_to_apply = L"Wybierz wyposażenie, które chcesz zastosować",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"Nadpisać model budowy [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"Wczytać model budowy [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Wyposażenie",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Puste miejsce",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Oręż",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Strzały/bełty",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Zbroja",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talizmany",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Bez broni",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Brak",
         // Machine translated
         .loadout_book = L"Podręcznik walki",
     }},

    // porbr / Português-Brasil (Portuguese - Brazil)
    {"brazilian",
     {
         .manage_loadouts = L"Equipamentos de Maculado",
         .save_loadout = L"Salvar equipamentos",
         .apply_loadout = L"Aplicar equipamentos",
         .select_loadout_to_save = L"Selecione um slot para salvar seus equipamentos atuais",
         .select_loadout_to_apply = L"Selecione um equipamento para aplicar",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"Substituir [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"Carregar [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Equipamento",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Espaço vazio",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Armas",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Flechas/Setas",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Armadura",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismãs",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Desarmado",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Nenhuma",
         // Machine Translated
         .loadout_book = L"Livro de luta",
     }},

    // rusru / Русский (Russian)
    {"russian",
     {
         .manage_loadouts = L"Снаряжение Погасшего",
         // TODO: translate
         .save_loadout = L"Save loadout",
         // TODO: translate
         .apply_loadout = L"Apply loadout",
         // TODO: translate
         .select_loadout_to_save = L"Select a slot to save your current loadout",
         // TODO: translate
         .select_loadout_to_apply = L"Select a loadout to apply",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"Перезаписать сборку [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"Загрузить сборку [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Снаряжение",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Пустая ячейка",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Оружие",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Стрелы/болты",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Доспехи",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Талисманы",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Без оружия",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Нет",
         // TODO: translate
         .loadout_book = L"Tarnished Combat Manual",
     }},

    // spaar / Español-Latinoamérica (Spanish - Latin America)
    {"latam",
     {
         .manage_loadouts = L"Equipamientos de Tiznado",
         // Machine translated
         .save_loadout = L"Guardar equipamientos",
         // Machine translated
         .apply_loadout = L"Aplicar equipamientos",
         // Machine translated
         .select_loadout_to_save = L"Selecciona una ranura para guardar tus equipamientos actuales",
         // Machine translated
         .select_loadout_to_apply = L"Selecciona un equipamiento para aplicarlo",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"¿Quieres reemplazar [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"¿Quieres cargar [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Equipamiento",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Espacio vacío",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Piezas de armamento",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Flechas/Virotes",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Armadura",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismanes",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Sin arma",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Ninguno",
         // Manually translated
         .loadout_book = L"Libro de lucha",
     }},

    // spaes / Español-España (Spanish - Spain)
    {"spanish",
     {
         .manage_loadouts = L"Equipamientos de Sinluz",
         // Machine translated
         .save_loadout = L"Guardar equipamientos",
         // Machine translated
         .apply_loadout = L"Aplicar equipamientos",
         // Machine translated
         .select_loadout_to_save = L"Selecciona una ranura para guardar tus equipamientos actuales",
         // Machine translated
         .select_loadout_to_apply = L"Selecciona un equipamiento para aplicarlo",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"¿Sobrescribir [<?itemName?>]?",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"¿Cargar [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"Equipamiento",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"Espacio vacío",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"Armamento",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"Flechas/saetas",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"Armadura",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"Talismanes",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"Sin armas",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"Ninguno",
         // Manually translated
         .loadout_book = L"Libro de lucha",
     }},

    // thath / ไทย (Thai)
    {"thai",
     {
         .manage_loadouts = L"ชุดอุปกรณ์",
         // TODO: translate
         .save_loadout = L"Save loadout",
         // TODO: translate
         .apply_loadout = L"Apply loadout",
         // TODO: translate
         .select_loadout_to_save = L"Select a slot to save your current loadout",
         // TODO: translate
         .select_loadout_to_apply = L"Select a loadout to apply",
         // TODO: translate
         .save_loadout_prompt = L"Overwrite [<?itemName?>] with current loadout?",
         // TODO: translate
         .apply_loadout_prompt = L"Apply [<?itemName?>]?",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"ชุดอุปกรณ์",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"เขียนได้",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"อาวุธ",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"ลูก​ธนู/​ลูกดอก",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"เครือง​ปองกัน",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"เครืองราง",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"มือ​เปล่า",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"ไม่มี",
         // TODO: translate
         .loadout_book = L"Tarnished Combat Manual",
     }},

    // zhocn / 简体中文 (Simplified Chinese)
    {"schinese",
     {
         .manage_loadouts = L"兵种配置",
         // Machine translated
         .save_loadout = L"保存兵种配置",
         // Machine translated
         .apply_loadout = L"应用兵种配置",
         // Machine translated
         .select_loadout_to_save = L"选择一个插槽以保存当前兵种配置",
         // Machine translated
         .select_loadout_to_apply = L"选择要应用的兵种配置",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"覆盖〈<?itemName?>〉",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"读取〈<?itemName?>〉",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"配置",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"可记录",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"武器",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"箭／弩箭",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"防具",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"护符",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"徒手",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"无",
         // Machine translated
         .loadout_book = L"战斗手册",
     }},

    // zhotw / 繁體中文 (Traditional Chinese)
    {"tchinese",
     {
         .manage_loadouts = L"裝備清單",
         // Machine translated
         .save_loadout = L"儲存裝備清單",
         // Machine translated
         .apply_loadout = L"裝備清單",
         // Machine translated
         .select_loadout_to_save = L"選擇插槽以儲存當前裝備清單",
         // Machine translated
         .select_loadout_to_apply = L"選擇要套用的裝備清單",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206001 "Overwrite [<?assembleDataName?>]?"
         .save_loadout_prompt = L"覆蓋〈<?itemName?>〉",
         // (ARMORED CORE VI) FNR_ダイアログ.fmg 206002 "Load [<?assembleDataName?>]?"
         .apply_loadout_prompt = L"讀取〈<?itemName?>〉",
         // (TF2) TF_ImportPreview_LoadoutLabel "Loadout"
         .loadout = L"裝備欄",
         // (Sekiro) NTC_メニューテキスト.fmg 292017 "Empty Slot"
         .empty_slot = L"可記錄",
         // (ELDEN RING) GR_MenuText.fmg 40550 "Armaments"
         .armaments = L"武器",
         // (ELDEN RING) GR_MenuText.fmg 40511 "Arrows/Bolts"
         .arrows_bolts = L"箭／弩箭",
         // (ELDEN RING) GR_MenuText.fmg 40551 "Armor"
         .armor = L"防具",
         // (ELDEN RING) GR_MenuText.fmg 40509 "Talismans"
         .talismans = L"護符",
         // (ELDEN RING) WeaponName.fmg 110000 "Unarmed"
         .unarmed = L"徒手",
         // (ELDEN RING) GR_MenuText.fmg 50000 "None"
         .none = L"無",
         // Machine translated
         .loadout_book = L"戰鬥手冊",
     }},
};
