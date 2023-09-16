#if defined _adminsys_included
    #endinput
#endif
#define _adminsys_included

/*
    check libs
*/

#if !defined mysql_included || !defined cache_get_value_name
    #error "Please included <a_mysql.inc> version R40+ before include <admin_sys.inc>" 
#endif

#if !defined _FOREACH_LOCAL_VERSION
    #error "Please included <foreach.inc> before include <admin_sys.inc>"
#endif

#if !defined CMD
    #error "Please include <Pawn.CMD.inc> or <dc_cmd.inc> to correct work."
#endif

/*
    definitions
*/

#if !defined DB_USERS_ADMINS
    #define DB_USERS_ADMINS "users_admins"
#endif

#if !defined DB_USERS
    #define DB_USERS "users"
#endif

#if !defined HEX_COLOR_ADMIN_DEFAULT
	#define HEX_COLOR_ADMIN_DEFAULT 0xFFFFFFFF
#endif

#if !defined HEX_COLOR_ADMIN_CHAT
    #define HEX_COLOR_ADMIN_CHAT 0xFF6347AA
#endif

#if !defined HEX_COLOR_ADMIN_SUCCESS
    #define HEX_COLOR_ADMIN_SUCCESS 0x46BD38ff
#endif

#if !defined HEX_COLOR_ADMIN_ERROR
    #define HEX_COLOR_ADMIN_ERROR 0xBA2821FF
#endif 

#if !defined HEX_COLOR_ADMIN_NOTE 
    #define HEX_COLOR_ADMIN_NOTE 0xFF9945FF
#endif 

#if !defined SetPlayerUserID
    #define SetPlayerUserID(%0,%1) g_player_data[%0][E_PLAYER_DATA_ID] = %1
#endif

#if !defined GetPlayerUserID
    #define GetPlayerUserID(%0) g_player_data[%0][E_PLAYER_DATA_ID] 
#endif

#if !defined GetPlayerNameEx
    #define GetPlayerNameEx(%0) g_player_data[%0][E_PLAYER_DATA_NAME] 
#endif

#if !defined MAX_ADMIN_PASSWORD_LENGTH
    #define MAX_ADMIN_PASSWORD_LENGTH 32
#endif

#if !defined MIN_ADMIN_PASSWORD_LENGTH
    #define MIN_ADMIN_PASSWORD_LENGTH 6
#endif

#if !defined MAX_ADMINS_COUNT
	#define MAX_ADMINS_COUNT (50)
#endif

#if !defined MAX_ADMIN_LEVEL
	#define MAX_ADMIN_LEVEL (6)
#endif

#if !defined MIN_ADMIN_LEVEL
	#define MIN_ADMIN_LEVEL (1)
#endif

#if !defined MAX_ADMIN_PASSWORD_HASH_LENGTH
    #define MAX_ADMIN_PASSWORD_HASH_LENGTH (64)
#endif

#if !defined MAX_ADMIN_LOGIN_ATTEMPTS
    #define MAX_ADMIN_LOGIN_ATTEMPTS (5)
#endif

#if !defined MAX_ADMIN_COMMAND_COUNT
    #define MAX_ADMIN_COMMAND_COUNT (41)
#endif

#if !defined MAX_ADMIN_COMMAND_NAME_LENGTH
    #define MAX_ADMIN_COMMAND_NAME_LENGTH (20)
#endif

#if !defined MAX_ADMIN_COMMAND_DESC_LENGTH
    #define MAX_ADMIN_COMMAND_DESC_LENGTH (60)
#endif

#if !defined MAX_ADMIN_COMMAND_ARG_LENGTH
    #define MAX_ADMIN_COMMAND_ARG_LENGTH (60)
#endif

#if !defined _ADMINSYS_STATIC_ENUM
    #if defined __PawnBuild
		#define _ADMINSYS_STATIC_ENUM static
	#else
		#define _ADMINSYS_STATIC_ENUM
	#endif
#endif

#if !defined MAX_ADMIN_PREFIX_LENGTH
    #define MAX_ADMIN_PREFIX_LENGTH 32
#endif

#if !defined MAX_ADMIN_PREFIX_COLOR_LENGTH
    #define MAX_ADMIN_PREFIX_COLOR_LENGTH 8
#endif

/*
    vars
*/

static 
    Iterator: AdminPlayers<MAX_ADMINS_COUNT>,
    MySQL: dbHandle;

/*
    enums
*/

_ADMINSYS_STATIC_ENUM enum {
	DIALOG_ADMIN_REGISTER = 7578,
	DIALOG_ADMIN_LOGIN,
	DIALOG_ADMIN_HELP,
	DIALOG_ADMIN_HELP_RETURN,
	DIALOG_ADMIN_SHOW_STATS,
	DIALOG_ADMIN_OFFLINE_LIST,
	DIALOG_ADMIN_ONLINE_LIST,
	DIALOG_ADMIN_WEAPON_INFO
}

_ADMINSYS_STATIC_ENUM enum {
    ADMIN_COMMAND_LEVEL_0,
    ADMIN_COMMAND_LEVEL_1,
    ADMIN_COMMAND_LEVEL_2,
    ADMIN_COMMAND_LEVEL_3,
    ADMIN_COMMAND_LEVEL_4, 
    ADMIN_COMMAND_LEVEL_5, 
    ADMIN_COMMAND_LEVEL_6
}

_ADMINSYS_STATIC_ENUM enum E_PLAYER_ADMIN_STRUCT {
    E_PLAYER_ADMIN_ID,
    E_PLAYER_ADMIN_LEVEL, 
    E_PLAYER_ADMIN_PASSWORD[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1],
    E_PLAYER_ADMIN_SALT[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1],
    E_PLAYER_ADMIN_PREFIX[MAX_ADMIN_PREFIX_LENGTH + 1],
    E_PLAYER_ADMIN_PREFIX_COLOR[MAX_ADMIN_PREFIX_COLOR_LENGTH + 1],
    E_PLAYER_ADMIN_ATTEMPTS
};
static g_player_admin[MAX_PLAYERS][E_PLAYER_ADMIN_STRUCT];

static const NULL_g_player_admin[E_PLAYER_ADMIN_STRUCT] = {
	0,
	0,
	EOS,
	EOS,
	EOS,
	EOS,
    MAX_ADMIN_LOGIN_ATTEMPTS
};

_ADMINSYS_STATIC_ENUM enum E_PLAYER_DATA_STRUCT {
    E_PLAYER_DATA_ID,
    E_PLAYER_DATA_NAME[MAX_PLAYER_NAME + 1]
};
static g_player_data[MAX_PLAYERS][E_PLAYER_DATA_STRUCT];

static const NULL_g_player_data[E_PLAYER_DATA_STRUCT] = {
    0,
    EOS
};

_ADMINSYS_STATIC_ENUM enum E_ADMIN_COMMAND_STRUCT {
    E_ADMIN_COMMAND_LEVEL,
    E_ADMIN_COMMAND_NAME[MAX_ADMIN_COMMAND_NAME_LENGTH + 1],
    E_ADMIN_COMMAND_ARG[MAX_ADMIN_COMMAND_ARG_LENGTH + 1],
    E_ADMIN_COMMAND_DESC[MAX_ADMIN_COMMAND_DESC_LENGTH + 1]
};
static const g_admin_command[MAX_ADMIN_COMMAND_COUNT][E_ADMIN_COMMAND_STRUCT] = {
	{ADMIN_COMMAND_LEVEL_1, "/admin", "Текстовое сообщение", "Написать в чат всем администраторам"},
	{ADMIN_COMMAND_LEVEL_1, "/aduty", "Аргументы отсутствуют", "Авторизация/Регистрация в панели администратора"},
	{ADMIN_COMMAND_LEVEL_1, "/ahelp", "Аргументы отсутствуют", "Список команд администора с подробным описанием"},
    {ADMIN_COMMAND_LEVEL_1, "/answer", "ID игрока / часть NickName | Текст", "Ответить игроку"},
    {ADMIN_COMMAND_LEVEL_1, "/eject", "ID игрока / часть NickName", "Высадить игрока из транспорта"},
    {ADMIN_COMMAND_LEVEL_1, "/show_stats", "ID игрока / часть NickName", "Показать статистику игрока"},
	{ADMIN_COMMAND_LEVEL_2, "/admins", "Аргументы отсутствуют", "Список администрации в сети"},
	{ADMIN_COMMAND_LEVEL_2, "/goto", "ID игрока / часть NickName", "Телепортироваться к игроку"},
    {ADMIN_COMMAND_LEVEL_2, "/asay", "Текст", "Написать в общий чат"},
    {ADMIN_COMMAND_LEVEL_2, "/freeze", "ID игрока / часть NickName", "Заморозить игрока"},
    {ADMIN_COMMAND_LEVEL_2, "/unfreeze", "ID игрока / часть NickName", "Разморозить игрока"},
    {ADMIN_COMMAND_LEVEL_2, "/spawn_veh", "ID транспорта", "Телепортировать транспорт на респавн"},
    {ADMIN_COMMAND_LEVEL_3, "/off_admins", "Аргументы отсутствуют", "Список администрации (OFFLINE)"},
    {ADMIN_COMMAND_LEVEL_3, "/gethere", "ID игрока / часть NickName", "Телепортировать игрока к себе"},
    {ADMIN_COMMAND_LEVEL_3, "/cc", "Аргументы отсутствуют", "Очистить чат"},
    {ADMIN_COMMAND_LEVEL_3, "/goto_pos", "[x] [y] [z]", "Телепортироваться на координаты"},
    {ADMIN_COMMAND_LEVEL_3, "/goto_veh", "ID транспорта", "Телепортироваться к транспорту"},
    {ADMIN_COMMAND_LEVEL_3, "/aspawn", "ID игрока / часть NickName", "Заспавнить игрока"},
	{ADMIN_COMMAND_LEVEL_4, "/give_weapon", "ID игрока / часть NickName | ID оружия | Кол-во патронов", "Выдача оружия для игрока"},
	{ADMIN_COMMAND_LEVEL_4, "/slap", "ID игрока / часть NickName | Причина", "Дать пинка игроку"},
	{ADMIN_COMMAND_LEVEL_4, "/setint", "ID игрока / часть NickName | Интерьер", "Установить игроку интерьер"},
	{ADMIN_COMMAND_LEVEL_4, "/setvw", "ID игрока / часть NickName | Виртуальный мир", "Установить игроку виртуальный мир"},
    {ADMIN_COMMAND_LEVEL_4, "/weapon", "ID игрока / часть NickName", "Посмотреть информацию об оружии игрока"},
    {ADMIN_COMMAND_LEVEL_4, "/repair_veh", "ID транспорта", "Отремонтировать транспорт"},
    {ADMIN_COMMAND_LEVEL_4, "/set_skin", "ID игрока / часть NickName | ID скина", "Установить скин игроку"},
    {ADMIN_COMMAND_LEVEL_4, "/set_health", "ID игрока / часть NickName | Здоровье", "Установить здоровье игроку"},
    {ADMIN_COMMAND_LEVEL_4, "/set_armour", "ID игрока / часть NickName | Броня", "Установить броню игроку"},
    {ADMIN_COMMAND_LEVEL_4, "/set_money", "ID игрока / часть NickName | Деньги", "Установить деньги игроку"},
    {ADMIN_COMMAND_LEVEL_4, "/set_score", "ID игрока / часть NickName | Очки", "Установить очки (уровень) игроку"},
    {ADMIN_COMMAND_LEVEL_5, "/set_weather", "Номер погоды", "Установить погоду"},
    {ADMIN_COMMAND_LEVEL_5, "/set_time", "Время суток", "Установить время суток"},
	{ADMIN_COMMAND_LEVEL_5, "/create_admin", "ID игрока / часть NickName", "Создать нового администратора"},
	{ADMIN_COMMAND_LEVEL_5, "/edit_admin", "ID игрока / часть NickName", "Редактировать уровень администратора"},
	{ADMIN_COMMAND_LEVEL_5, "/remove_admin", "ID игрока / часть NickName", "Снять администратора"},
	{ADMIN_COMMAND_LEVEL_6, "/off_create_admin", "Имя игрока", "Создать нового администратора (OFFLINE)"},
	{ADMIN_COMMAND_LEVEL_6, "/off_edit_admin", "Имя игрока | Уровень", "Редактировать уровень администратора (OFFLINE)"},
	{ADMIN_COMMAND_LEVEL_6, "/off_remove_admin", "Имя игрока", "Снять администратора (OFFLINE)"},
    {ADMIN_COMMAND_LEVEL_6, "/veh", "ID транспорта | цвет 1 | цвет 2", "Создать транспорт"},
    {ADMIN_COMMAND_LEVEL_6, "/dveh", "ID транспорта", "Удалить транспорт"},
    {ADMIN_COMMAND_LEVEL_6, "/set_prefix", "ID игрока / часть NickName | Наименование", "Установить префикс администратора"},
    {ADMIN_COMMAND_LEVEL_6, "/prefix_color", "ID игрока / часть NickName | ID цвета", "Изменить цвет префикса администратора"}
};

/*  
    forwards
*/

forward bool: admin_IsPlayerLogin(const playerid);
forward bool: admin_IsPlayerValid(const playerid);
forward bool: admin_IsPasswordCharsValid(const string[]);
forward bool: admin_IsPlayerPasswordEqual(password[], const expected_hash[], salt[]);

/*  
    hook OnGameModeInit
*/

public OnGameModeInit()
{
    admin_Init();
    
    #if defined adminsys_OnGameModeInit
        return adminsys_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit adminsys_OnGameModeInit
#if defined adminsys_OnGameModeInit
    forward adminsys_OnGameModeInit();
#endif

/*  
	hook OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{ 
    admin_ClearPlayerData(playerid);
    
    GetPlayerName(playerid, g_player_data[playerid][E_PLAYER_DATA_NAME], MAX_PLAYER_NAME);

    admin_LoadPlayerUserID(playerid);

    #if defined adminsys_OnPlayerConnect
        return adminsys_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect adminsys_OnPlayerConnect
#if	defined adminsys_OnPlayerConnect
	forward adminsys_OnPlayerConnect(playerid);
#endif

/* 
    hook OnPlayerDisconnect 
*/

public OnPlayerDisconnect(playerid, reason)
{
    admin_ClearPlayerAdminData(playerid);

    #if defined adminsys_OnPlayerDisconnect
        return adminsys_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect adminsys_OnPlayerDisconnect
#if defined adminsys_OnPlayerDisconnect
    forward adminsys_OnPlayerDisconnect(playerid, reason);
#endif

/* 
    hook OnDialogResponse
*/

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid) {
		case DIALOG_ADMIN_REGISTER: {
            if (!response) {
                return 0;
            }

            if (!(MIN_ADMIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_ADMIN_PASSWORD_LENGTH)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_ERROR, 
                    !"Длина пароля должна быть не менее \
                    "#MIN_ADMIN_PASSWORD_LENGTH" и не более \
                    "#MAX_ADMIN_PASSWORD_LENGTH" символов."
                );
                return admin_PlayerRegisterDialog(playerid);
            }

            if (!admin_IsPasswordCharsValid(inputtext)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_ERROR, 
                    !"Пароль может состоять только из латинских символов и цифр (aA-zZ, 0-9)."
                );
                return admin_PlayerRegisterDialog(playerid);
            }
            
            admin_GeneratePlayerPassword(playerid, inputtext);

            admin_SavePlayerEx(playerid, "name", g_player_data[playerid][E_PLAYER_DATA_NAME]);
            admin_SavePlayerEx(playerid, "password", g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD]);
            admin_SavePlayerEx(playerid, "salt", g_player_admin[playerid][E_PLAYER_ADMIN_SALT]);

            Iter_Add(AdminPlayers, playerid);

            new string[53 + (- 2 + MAX_ADMIN_PASSWORD_LENGTH) + 1];

            format(string, sizeof string, "Ваш новый пароль от панели администратора: {FFFFFF}%s",
                    inputtext
            );
            SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);
        }
        case DIALOG_ADMIN_LOGIN: {
            if (!response) {
                return 0;
            }

            if (!(MIN_ADMIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_ADMIN_PASSWORD_LENGTH)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_ERROR, 
                    !"Пароль должен быть не менее \
                    "#MIN_ADMIN_PASSWORD_LENGTH" и не более \
                    "#MAX_ADMIN_PASSWORD_LENGTH" символов в длину."
                );
                return admin_ShowPlayerLoginDialog(playerid);
            }

            if (!admin_IsPlayerPasswordEqual(inputtext, g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], g_player_admin[playerid][E_PLAYER_ADMIN_SALT])) {
                if (--g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] <= 0) {
                    SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы исчерпали все попытки для ввода пароля и были кикнуты.");
                    return Kick(playerid);
                }

                SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Пароли не совпадают. Повторите попытку.");
                return admin_ShowPlayerLoginDialog(playerid);
            } 

            Iter_Add(AdminPlayers, playerid);
           
            SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, !"Вы успешно авторизовались в панели администратора.");
        }
        case DIALOG_ADMIN_HELP: {
            if (!response) {
                return 0;
            }

            new
                string[(20 + MAX_ADMIN_COMMAND_NAME_LENGTH + MAX_ADMIN_COMMAND_DESC_LENGTH + MAX_ADMIN_COMMAND_ARG_LENGTH) * sizeof g_admin_command],
                header_string[50 + (- 2 + 1) + 1];

            string = "Команда\tАргументы\tОписание\n";

            for (new i = 0; i < sizeof g_admin_command; ++i) {
                if (g_admin_command[i][E_ADMIN_COMMAND_LEVEL] != listitem + 1) {
                    continue;
                }

                format(string, sizeof string, "%s%s\t%s\t%s\n",
                        string,
                        g_admin_command[i][E_ADMIN_COMMAND_NAME],
                        g_admin_command[i][E_ADMIN_COMMAND_ARG],
                        g_admin_command[i][E_ADMIN_COMMAND_DESC]
                );
            }

            format(header_string, sizeof header_string, "{5D9978}Команды администратора: {FFFFFF}%d уровня", 
                    admin_GetPlayerLevel(playerid)
            );

            ShowPlayerDialog(
                playerid,
                DIALOG_ADMIN_HELP_RETURN, 
                DIALOG_STYLE_TABLIST_HEADERS,
                header_string,
                string,
                !"Закрыть", !"Назад"
            );
        }
        case DIALOG_ADMIN_HELP_RETURN: {
            if (!response) {
                admin_ShowPlayerHelpDialog(playerid);
            }
        }
    }

    #if defined adminsys_OnDialogResponse
        return adminsys_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 0;
    #endif
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse adminsys_OnDialogResponse
#if defined adminsys_OnDialogResponse
    forward adminsys_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

/*  
    Get / Set
*/

stock admin_GetPlayerAdminID(const playerid) 
{
    return g_player_admin[playerid][E_PLAYER_ADMIN_ID];
}

stock admin_SetPlayerAdminID(const playerid, const value) 
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ID] = value;
}

stock admin_GetPlayerLevel(const playerid)
{
    return g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL];
}

stock admin_SetPlayerLevel(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL] = value;
}

stock admin_GetPlayerPassword(const playerid, output_string[], size = sizeof(output_string))
{
    strcat((output_string[0] = EOS, output_string), g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], size);

    return 1;
}

stock admin_SetPlayerPassword(const playerid, const output_string[], size = sizeof(output_string))
{
    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], output_string, size);

    return 1;
}

stock admin_GetPlayerSalt(const playerid, output_string[], size = sizeof(output_string))
{
    strcat((output_string[0] = EOS, output_string), g_player_admin[playerid][E_PLAYER_ADMIN_SALT], size);

    return 1;
}

stock admin_SetPlayerSalt(const playerid, const output_string[], size = sizeof(output_string))
{
    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_SALT], output_string, size);

    return 1;
}

stock bool: admin_IsPlayerLogin(const playerid)
{
    return bool: (Iter_Contains(AdminPlayers, playerid));
}

stock admin_GetPlayerLoginAttempts(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] = value;  
}

stock admin_SetPlayerLoginAttempts(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] = value;  
}

/*  
    functions
*/

stock admin_SetMySQLConnectionHandle(MySQL: handle)
{
    dbHandle = handle;
}

stock admin_Init()
{
    new 
        Cache: cache_id = mysql_query(dbHandle, !"SHOW TABLES LIKE '"DB_USERS_ADMINS"'", true),
        tick = GetTickCount();

    if (!cache_num_rows()) {
        mysql_query(dbHandle,
            "\
                CREATE TABLE `"DB_USERS_ADMINS"` ( \
                    `id` int(11) NOT NULL AUTO_INCREMENT, \
                    `user_id` int(11) NOT NULL, \
                    `name` varchar(24) NOT NULL, \
                    `password` varchar(64) NOT NULL, \
                    `salt` varchar(64) NOT NULL, \
                    `level` int(2) NOT NULL, \
                    `reg_IP` int(10) unsigned NOT NULL, \
                    `reg_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP, \
                    `prefix_name` varchar(32) NOT NULL, \
                    `prefix_color` varchar(8) NOT NULL \
                    PRIMARY KEY (`id`) \
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8; \
            ", false
        );
        mysql_query(dbHandle,
            !"\
                CREATE INDEX \
                    user_id \
                ON \
                    `"DB_USERS_ADMINS"`(user_id) \
            ", false
        );
        mysql_query(dbHandle,
            !"\
                ALTER TABLE \
                    `"DB_USERS_ADMINS"`\
                ADD CONSTRAINT \
                    `users_admins_users_fk_1` \
                FOREIGN KEY \
                    (`user_id`) \
                REFERENCES \
                    `"DB_USERS"` (`id`) \
                ON DELETE CASCADE ON UPDATE CASCADE \
            ", false
        );

        printf("[Debug]: таблица `"DB_USERS_ADMINS"` инициализирована за %d мс.", GetTickCount() - tick);
    } 

    cache_delete(cache_id);

    return mysql_errno() == 0;
}

stock admin_SavePlayerEx(const playerid, const column_name[], const column_value[])
{
    static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET `%e`='%e' WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 20) + (- 2 + MAX_ADMIN_PASSWORD_HASH_LENGTH) + (- 2 + 11)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query, 
            column_name,
            column_value,
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    return mysql_errno() == 0;
}

stock admin_LoadPlayerUserID(playerid)
{
    static const query[] = "SELECT u.id FROM `"DB_USERS"` u WHERE u.name='%e' LIMIT 1";
	
    new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            GetPlayerNameEx(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadUserID", !"d", playerid);

    return mysql_errno() == 0;
}

stock admin_LoadPlayerData(const playerid)
{
	static const query[] = 
	"\
        SELECT * FROM `"DB_USERS_ADMINS"` a \
        LEFT JOIN `"DB_USERS"` u ON u.id=a.user_id \
        WHERE u.id=%d \
	";
	
    new fmt_query[sizeof(query) + (- 2 + 11)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            GetPlayerUserID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadAdminData", !"d", playerid);
    
    return mysql_errno() == 0;
}

stock admin_PlayerCreate(const playerid)
{
    static const query[] = 
    "\
        INSERT INTO `"DB_USERS_ADMINS"` \
        	(name, level, reg_ip) \
        VALUES \
        	('%e', %d, INET_ATON('%e')) \
    ";

    new 
        fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME) + (- 2 + 1) + (- 2 + 16)],
        ip_adress[16];

    GetPlayerIp(playerid, ip_adress, sizeof ip_adress);

    admin_SetPlayerLevel(playerid, MIN_ADMIN_LEVEL);

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query,
            GetPlayerNameEx(playerid),
            MIN_ADMIN_LEVEL,
            ip_adress
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadAdminID", !"d", playerid);

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Для регистрации пароля администратора, используйте команду - /aduty."
    );

    return mysql_errno() == 0;
}

stock admin_PlayerUpdate(const playerid)
{
	static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET level=%d WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 1) + (- 2 + 11)];

    format(
            fmt_query, 
            sizeof(fmt_query), 
            query,
            admin_GetPlayerLevel(playerid),
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_ERROR, 
        !"Ваш уровень администратора был изменён."
    );
    
	return mysql_errno() == 0;
}

stock admin_PlayerRemove(const playerid)
{
    static const query[] = 
    "\
        DELETE FROM `"DB_USERS_ADMINS"` WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 11)];

    format(
            fmt_query, 
            sizeof(fmt_query), 
            query,
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    admin_ClearPlayerAdminData(playerid);
    admin_ClearPlayerData(playerid);

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_ERROR, 
        !"Вы были сняты с должности администратора."
    );
    
    return mysql_errno() == 0;
}

stock admin_PlayerCreateOffline(const playerid, const name[])
{
	static const query[] =
    "\
    	SELECT u.id, u.name FROM `"DB_USERS"` u WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerCreateAdminOffline", !"ds", playerid, name);
	
	return mysql_errno() == 0;
}

stock admin_PlayerEditOffline(const playerid, const name[], const level)
{
	static const query[] =
    "\
    	SELECT u.name FROM `"DB_USERS_ADMINS"` u WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerEditAdminOffline", !"dsd", playerid, name, level);
	
	return mysql_errno() == 0;
}

stock admin_PlayerRemoveOffline(const playerid, const name[])
{
    static const query[] =
    "\
    	SELECT u.name FROM `"DB_USERS_ADMINS"` u WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerRemoveAdminOffline", !"ds", playerid, name);

    return mysql_errno() == 0;
}

stock admin_ClearPlayerData(const playerid)
{
    g_player_data[playerid] = NULL_g_player_data;
}

stock admin_ClearPlayerAdminData(const playerid)
{
    if (admin_IsPlayerLogin(playerid)) {
        Iter_Remove(AdminPlayers, playerid);

        g_player_admin[playerid] = NULL_g_player_admin;
    }

    return 1;
}

stock bool: admin_IsPlayerValid(const playerid) 
{
    return (g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL] != NULL_g_player_admin[E_PLAYER_ADMIN_LEVEL]);
}

stock admin_SendMessage(const color, const message[])
{
    foreach (new idx : AdminPlayers) {
        SendClientMessage(idx, color, message);
    }

    return 1;
}

stock admin_GetAccessCommand(const playerid, const level)
{
    return (admin_GetPlayerLevel(playerid) < level);
}

stock admin_PlayerRegisterDialog(playerid) 
{
    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_REGISTER, 
            DIALOG_STYLE_INPUT,
            !"{5D9978}Регистрация пароля",
            !"\t{FFFFFF}Придумайте пароль от панели администратора и введите его в диалоговое поле ниже.\n\n\
            {AFAFAF}– Примечание:\n\
            {AFAFAF}•{FFFFFF} Пароль должен быть не менее "#MIN_ADMIN_PASSWORD_LENGTH" и не более "#MAX_ADMIN_PASSWORD_LENGTH" символов в длину.\n\
            {AFAFAF}•{FFFFFF} Пароль может состоять только из латинских символов и цифр (aA-zZ, 0-9).\n\
            {AFAFAF}•{FFFFFF} Пример надёжного пароля: {AFAFAF}32asbe501Tg67FDDDw",
            !"Далее", !"Отмена"
	);
}

stock admin_ShowPlayerLoginDialog(playerid) 
{
    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_LOGIN, 
            DIALOG_STYLE_PASSWORD,
	    	!"{5D9978}Авторизация администратора",
		    !"{FFFFFF}Введите пароль от панели администратора в диалоговое поле ниже:",
		    !"Далее", !"Отмена"
	);
}

stock admin_ShowPlayerHelpDialog(const playerid) 
{
    new string[(12 + (- 2 + 1)) * MAX_ADMIN_LEVEL + 1];

    for (new i = 0; i < MAX_ADMIN_LEVEL; ++i) {
        format(string, sizeof string, "%s%d уровень\n", 
                string, 
                i + 1
        );
    }

    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_HELP, 
            DIALOG_STYLE_LIST,
            !"{5D9978}Выбор уровня администратора",
            string,
            !"Выбрать", !"Отмена"
	);
}

stock bool: admin_IsPlayerPasswordEqual(password[], const expected_hash[], salt[])
{
    new hash[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1];

    SHA256_PassHash(password, salt, hash, sizeof hash);

    return bool: (!strcmp(hash, expected_hash, false));
}

stock admin_GeneratePlayerPassword(const playerid, password[]) 
{
	static const random_letters[] = "TOBI8Uv4mZQ5QhuPWRWEtx2WIrqbkginKYfmGdGtyzQcIilwjfx3dkKGpg8RU5";

    new length = strlen(random_letters);
			
    for (new idx = 0; idx < MAX_ADMIN_PASSWORD_HASH_LENGTH; ++idx) {
        g_player_admin[playerid][E_PLAYER_ADMIN_SALT][idx] = random_letters[random(length)];
    }

    SHA256_PassHash(
        password, 
        g_player_admin[playerid][E_PLAYER_ADMIN_SALT], 
        g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], 
        MAX_ADMIN_PASSWORD_HASH_LENGTH + 1
    );

	return 1;
}

stock bool: admin_IsPasswordCharsValid(const string[]) 
{
    for (new idx = 0; string[idx] != '\0'; ++idx) {
		switch (string[idx]) {
			case 'a'..'z', 'A'..'Z', '0'..'9': {
				continue;
			}
			default: {
				return false;
			}
		}
	}

    return true;
}

stock admin_GetPlayerOnlineByID(const player_name[])
{
    foreach (new idx : AdminPlayers) {
        if (!strcmp(GetPlayerNameEx(idx), player_name)) {
            return idx;
        }
    }

    return INVALID_PLAYER_ID;
}

/*  
    MySQL callbacks
*/

@__OnPlayerLoadUserID(playerid);
@__OnPlayerLoadUserID(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }

    new user_id;
    
    cache_get_value_name_int(0, !"id", user_id);
    
    SetPlayerUserID(playerid, user_id);

    admin_LoadPlayerData(playerid);

    return 1;
}

@__OnPlayerLoadAdminData(playerid);
@__OnPlayerLoadAdminData(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }

    cache_get_value_name_int(0, !"id", g_player_admin[playerid][E_PLAYER_ADMIN_ID]);
    cache_get_value_name_int(0, !"level", g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL]);
    
    cache_get_value_name(0, !"password", g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], MAX_ADMIN_PASSWORD_HASH_LENGTH + 1);
    cache_get_value_name(0, !"salt", g_player_admin[playerid][E_PLAYER_ADMIN_SALT], MAX_ADMIN_PASSWORD_HASH_LENGTH + 1);
    cache_get_value_name(0, !"prefix_name", g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX], MAX_ADMIN_PREFIX_LENGTH + 1);
    cache_get_value_name(0, !"prefix_color", g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX_COLOR], MAX_ADMIN_PREFIX_COLOR_LENGTH + 1);

    if (isnull(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD])) {
        SendClientMessage(
            playerid, 
            HEX_COLOR_ADMIN_ERROR, 
            !"Вы вошли как незарегистрированный администратор."
        );
        SendClientMessage(
            playerid, 
            HEX_COLOR_ADMIN_ERROR, 
            !"Для регистрации пароля используйте - /aduty."
        );
    } else {
        admin_SetPlayerLoginAttempts(playerid, MAX_ADMIN_LOGIN_ATTEMPTS);

        static const message[] = 
        "\
            Вы вошли как администратор (%d) уровня. \
            Для авторизации используйте - /aduty. \
        ";

        new fmt_message[sizeof(message) + (- 2 + 2)];

        format(
                fmt_message, 
                sizeof(fmt_message), 
                message,
                admin_GetPlayerLevel(playerid)
        );
        SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, fmt_message);
    } 

    return 1;
}

@__OnPlayerLoadAdminID(playerid);
@__OnPlayerLoadAdminID(playerid)
{
    admin_SetPlayerAdminID(playerid, cache_insert_id());
    return 1;
}

@__OnPlayerLoadOfflineAdmins(playerid);
@__OnPlayerLoadOfflineAdmins(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }
    
    new
        dialog_string[20 + ((- 2 + MAX_PLAYER_NAME) + (- 2 + 1)) * MAX_ADMINS_COUNT + 1] = "Ник\tУровень\n",
        tmp_name[MAX_PLAYER_NAME + 1], 
        tmp_level;

    for (new idx = 0; idx < row_count; ++idx) {
        cache_get_value_name(idx, !"name", tmp_name);

        if (admin_GetPlayerOnlineByID(tmp_name) != INVALID_PLAYER_ID) {
            continue;
        }

        cache_get_value_name_int(idx, !"level", tmp_level);

        format(
                dialog_string,
                sizeof(dialog_string),
                "%s{5D9978}%s\t{FFFFFF}%d\n",
                dialog_string,
                tmp_name,
                tmp_level
        );
    }

    ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_OFFLINE_LIST, 
        DIALOG_STYLE_TABLIST_HEADERS,
		!"{5D9978}Администраторы (OFFLINE)",
		dialog_string,
		!"Закрыть", !""
	);
    return 1;
}

@__OnPlayerCreateAdminOffline(playerid, name[]);
@__OnPlayerCreateAdminOffline(playerid, name[])
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Аккаунт игрока не зарегистрирован в базе данных.");
    }
    
    new user_id;
    
    cache_get_value_name_int(0, !"id", user_id);

    static const query[] = 
    "\
        INSERT INTO `"DB_USERS_ADMINS"` \
            (user_id, name, level) \
        VALUES \
            (%d, '%e', %d) \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 11) + (- 2 + MAX_PLAYER_NAME) + (- 2 + 1)];
    
    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query,
            user_id,
            name,
            MIN_ADMIN_LEVEL
    );
    mysql_tquery(dbHandle, fmt_query, "", "");
    
    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Вы успешно создали нового администратора в режиме (OFFLINE)."
    );

    return 1;
}

@__OnPlayerEditAdminOffline(playerid, name[], level);
@__OnPlayerEditAdminOffline(playerid, name[], level)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Игрок не является активным администратором.");
    }

    static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET level=%d WHERE name='%e' LIMIT 1 \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 1) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query),
            query,
            level,
            name
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Вы успешно обновили уровень администратора в режиме (OFFLINE)."
    );

    return 1;
}

@__OnPlayerRemoveAdminOffline(playerid, name[]);
@__OnPlayerRemoveAdminOffline(playerid, name[])
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Игрок не является активным администратором.");
    }

    static const query[] = 
    "\
        DELETE FROM `"DB_USERS_ADMINS"` WHERE name='%e' LIMIT 1 \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Вы успешно сняли администратора в режиме (OFFLINE)."
    );

    return 1;
}

/*  
    commands
*/

CMD:aduty(playerid) 
{
    if (admin_GetPlayerLevel(playerid) == ADMIN_COMMAND_LEVEL_0) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не обладаете правами администратора.");
    }

    if (Iter_Count(AdminPlayers) > MAX_ADMINS_COUNT) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Превышен лимит администраторов на сервере.");
    }

    if (admin_IsPlayerLogin(playerid)) {
        Iter_Remove(AdminPlayers, playerid);
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы вышли из панели администратора.");
    }

    if (isnull(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD])) {
        admin_PlayerRegisterDialog(playerid);
    } else {
        admin_ShowPlayerLoginDialog(playerid);
    }

    return 1;
}

CMD:ahelp(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    admin_ShowPlayerHelpDialog(playerid);
    return 1;
}

CMD:create_admin(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, "Используйте: /create_admin [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы указали свой ID.");
    }
    
    if (admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок уже администратор.");
    }

    if (Iter_Count(AdminPlayers) > MAX_ADMINS_COUNT) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Превышен лимит администраторов на сервере.");
    }

    admin_PlayerCreate(targetid);
	return 1;
}

CMD:edit_admin(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        targetid = INVALID_PLAYER_ID,
        level;

    if (sscanf(params, !"ud", targetid, level)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, "Используйте: /edit_admin [ID игрока / часть NickName] [Уровень]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы указали свой ID.");
    }

	if (!admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не администратор.");
    }
    
    if (!(0 <= level <= MAX_ADMIN_LEVEL)) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Некорректный уровень администратора.");
    }
    
    if (admin_GetPlayerLevel(targetid) == level) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный администратор такого же уровня.");
    }
    
    admin_SetPlayerLevel(playerid, level);
    
    admin_PlayerUpdate(targetid);
    return 1;
}

CMD:remove_admin(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, "Используйте: /remove_admin [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы указали свой ID.");
    }
    
    if (!admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не администратор.");
    }

    admin_PlayerRemove(targetid);
    return 1;
}

CMD:admin(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

	if (isnull(params)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /admin [Текст]");
    }

    new string[MAX_CHATBUBBLE_LENGTH + 1];

    format(string, sizeof string, "[A] %s[%d]: %s",
            GetPlayerNameEx(playerid),
            playerid,
            params
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

	return 1;
}

CMD:admins(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        dialog_string[27 + ((- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 1)) * MAX_ADMINS_COUNT + 1] = "Ник\tУровень\n",
        header_string[41 + (- 2 + 2) + 1];

	foreach (new idx : AdminPlayers) {
        format(dialog_string, sizeof dialog_string, "%s{5D9978}%s[%d]\t{FFFFFF}%d\n",
                dialog_string,
                GetPlayerNameEx(idx),
                idx,
                admin_GetPlayerLevel(idx)
        );
    }

	format(header_string, sizeof header_string, "{BA2821}Администраторы в сети: {FFFFFF}%d",
            Iter_Count(AdminPlayers)
    );

    if (!Iter_Count(AdminPlayers)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"В данный момент нет администраторов в сети.");
    }

	ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_ONLINE_LIST, 
        DIALOG_STYLE_TABLIST_HEADERS,
		header_string,
		dialog_string,
		!"Закрыть", !""
	);

	return 1;
}

CMD:off_admins(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    mysql_tquery(
            dbHandle, 
            !"SELECT ua.name, ua.level \
            FROM `"DB_USERS_ADMINS"` ua \
            WHERE ua.level>0", 
            !"@__OnPlayerLoadOfflineAdmins", 
            !"d", 
            playerid
    );
    return 1;
}

#define MAX_PLAYER_NAME_LENGTH 24

CMD:off_create_admin(playerid, const params[]) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new target_name[MAX_PLAYER_NAME_LENGTH];

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LENGTH "]", target_name)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /off_create_admin [Имя игрока]");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Игрок находится в сети.");
    }

    admin_PlayerCreateOffline(playerid, target_name);
    return 1;
}

CMD:off_edit_admin(playerid, const params[]) 
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new target_name[MAX_PLAYER_NAME_LENGTH], level;

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LENGTH "]d", target_name, level)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /off_edit_admin [Имя игрока] [Уровень]");
    }

    if (!(0 <= level <= MAX_ADMIN_LEVEL)) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Некорректный уровень администратора.");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Игрок находится в сети.");
    }

    admin_PlayerEditOffline(playerid, target_name, level);
    return 1;
}

CMD:off_remove_admin(playerid, const params[]) 
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }
    
    new target_name[MAX_PLAYER_NAME_LENGTH];

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LENGTH "]", target_name)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /off_remove_admin [Имя игрока]");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Игрок находится в сети.");
    }

    admin_PlayerRemoveOffline(playerid, target_name);
    return 1;
}

#undef MAX_PLAYER_NAME_LENGTH

CMD:give_weapon(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        targetid = INVALID_PLAYER_ID,
        weaponid,
        ammo;
    
	if (sscanf(params, !"udd", targetid, weaponid, ammo)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /give_weapon [ID игрока / часть NickName] [ID оружия] [Кол-во патронов]");
    }
    
    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (!(1 <= weaponid <= 46)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Укажите корректный ID оружия от 1-го до 46-го.");
    }
	
    if (!(1 <= ammo <= 999)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Укажите корректное количество патронов от 1 до 999.");
    }

	GivePlayerWeapon(targetid, weaponid, ammo);

    new string[59 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 2) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы выдали %s[%d] оружие #%d с %d боеприпасов.", 
            GetPlayerNameEx(targetid), 
            targetid, 
            weaponid, 
            ammo
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

    format(string, sizeof string, "Администратор %s[%d] выдал Вам оружие #%d с %d боеприпасов.", 
            GetPlayerNameEx(playerid),
            playerid,
            weaponid, 
            ammo
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:slap(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"us[128]", targetid, params)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /slap [ID игрока / часть NickName] [Причина]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (admin_GetPlayerLevel(playerid) <= admin_GetPlayerLevel(targetid) ) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не можете применить это действие к администратору.");
    }

	new
		Float: target_x,
		Float: target_y,
		Float: target_z;

	GetPlayerPos(targetid, target_x, target_y, target_z);

	SetPlayerPos(targetid, target_x, target_y, target_z + 8.0);

    PlayerPlaySound(targetid, 1130, 0.0, 0.0, 0.0);

    new string[52 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + (- 2 + 128) + 1];

    format(string, sizeof string, "%s[%d] подкинул игрока %s[%d]. Причина: %s", 
            GetPlayerNameEx(playerid),
            playerid,
            GetPlayerNameEx(targetid),
            targetid,
            params
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    format(string, sizeof string, "Администратор %s[%d] подкинул вас. Причина: %s", 
            GetPlayerNameEx(playerid),
            playerid,
            params
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:goto(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /goto [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

	if (targetid == playerid) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не можете телепортироваться к самому себе.");
    }

	new
		Float: target_x,
		Float: target_y,
		Float: target_z,
		target_vw = GetPlayerVirtualWorld(targetid),
		target_int = GetPlayerInterior(targetid);

	GetPlayerPos(targetid, target_x, target_y, target_z);

	SetPlayerPos(playerid, target_x + 1.0, target_y + 1.0, target_z);
    SetPlayerInterior(playerid, target_int);
    SetPlayerVirtualWorld(playerid, target_vw);

	new string[47 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + 1];

    format(string, sizeof string, "%s[%d] телепортировался к %s[%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    format(string, sizeof string, "Вы телепортировались к %s[%d].", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:gethere(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /gethere [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не можете телепортировать самого себя.");
    }

	new
		Float: player_x,
		Float: player_y,
		Float: player_z,
		player_vw = GetPlayerVirtualWorld(playerid),
		player_int = GetPlayerInterior(playerid);

	GetPlayerPos(playerid, player_x, player_y, player_z);

	SetPlayerPos(targetid, player_x + 1.0, player_y + 1.0, player_z);
    SetPlayerInterior(targetid, player_vw);
    SetPlayerVirtualWorld(targetid, player_int);

	new string[36 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + 1];

    format(string, sizeof string, "%s[%d] телепортировал к себе %s[%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    format(string, sizeof string, "Вы телепортировали к себе %s[%d].", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] телепортировал Вас к себе.", 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);
    
	return 1;
}

CMD:setint(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        targetid = INVALID_PLAYER_ID,
        interiorid;

    if (sscanf(params, !"ud", targetid, interiorid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /setint [ID игрока / часть NickName] [ID интерьера]");
    }

	SetPlayerInterior(targetid, interiorid);
	
	new string[52 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] интерьер ID [%d].", 
            GetPlayerNameEx(targetid),
            targetid,
            interiorid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам интерьер ID [%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            interiorid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:setvw(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        targetid = INVALID_PLAYER_ID,
        worldid;

    if (sscanf(params, !"ud", targetid, worldid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /setvw [ID игрока / часть NickName] [Виртуальный мир]");
    }

	SetPlayerVirtualWorld(targetid, worldid);
	
	new string[56 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] виртуальный мир [%d].", 
            GetPlayerNameEx(targetid),
            targetid,
            worldid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам виртуальный мир [%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            worldid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:set_weather(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new weatherid;

    if (sscanf(params, !"d", weatherid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_weather [Номер погоды]");
    }

    if (!(0 <= weatherid <= 20)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"ID погоды должен быть от 0 до 20.");
    }

    foreach (new i : Player) {
        SetPlayerWeather(i, weatherid);
    }

    new string[52 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 2) + 1];

    format(string, sizeof string, "Администратор %s[%d] изменил погоду сервера на [%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            weatherid
    );
    SendClientMessageToAll(HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:set_time(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new time;

    if (sscanf(params, !"d", time)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_time [Время суток]");
    }

    if (!(0 <= time <= 23)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"ID погоды должен быть от 0 до 23.");
    }

    foreach (new i : Player) {
        SetPlayerTime(i, time, 0);
    }

    new string[51 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 2) + 1];

    format(string, sizeof string, "Администратор %s[%d] изменил время сервера на [%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            time
    );
    SendClientMessageToAll(HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:asay(playerid, const params[]) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }
    
    if (isnull(params)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /asay [Текст]");
    }

    new string[MAX_CHATBUBBLE_LENGTH + 1];

    format(string, sizeof string, "Администратор %s[%d]: %s",
            GetPlayerNameEx(playerid),
            playerid,
            params
    );
    SendClientMessageToAll(HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:answer(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

	new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"us[128]", targetid, params)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /answer [ID игрока / часть NickName] [Текст]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

	if (targetid == playerid) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не можете ответить самому себе!");
    }

	new string[38 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3 ) * 2 + (- 2 + 128) + 1];

	format(string, sizeof string, "<- Администратор %s[%d] для %s[%d]: %s",
            GetPlayerNameEx(playerid),
            playerid,
            GetPlayerNameEx(targetid),
            targetid,
            params
    );
	admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

	format(string, sizeof string, "-> Администратор %s[%d] ответил Вам: %s",
            GetPlayerNameEx(playerid),
            playerid,
            params
    );
	SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:cc(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    static const 
    	string[] = "",
    	message[] = "Администратор %s[%d] очистил чат.";
    
    new 
    	i = 100,
    	fmt_string[sizeof(message) + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3)];

    #if __Pawn < 0x030A
            { if(0 == i) SendClientMessageToAll(0, string); }
    #endif

    #emit    push.c        string
    #emit    push.c        0xFFFFFFFF
    #emit    push.c        8

    do
    {
    	#emit    sysreq.c    SendClientMessageToAll
    }
    while(--i);

    #emit    stack        12

    format(fmt_string, sizeof fmt_string, message, 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessageToAll(HEX_COLOR_ADMIN_CHAT, fmt_string);

    return 1;
}

CMD:goto_pos(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

	new 
        Float: x, 
        Float: y, 
        Float: z;

	if (sscanf(params, !"fffdd", x, y, z)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /goto_pos [X] [Y] [Z]");
    }

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);

        SetVehiclePos(vehicleid, x, y, z);
        PutPlayerInVehicle(playerid, vehicleid, 0);
    }
    
    SetPlayerPos(playerid, x, y, z);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);
	
    return 1;
}

CMD:eject(playerid, const const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /eject [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    if (!IsPlayerInAnyVehicle(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не находится в транспорте.");
    }

    RemovePlayerFromVehicle(targetid);

    new string[48 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы высадили %s[%d] из транспорта.", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] высадили Вас из транспорта.", 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:freeze(playerid, const params[]) 
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /freeze [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

	TogglePlayerControllable(targetid, 0);

    new string[35 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы заморозили %s[%d].", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] заморозил Вас.", 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:unfreeze(playerid, const params[]) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /unfreeze [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

	TogglePlayerControllable(targetid, 1);

    new string[35 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы разморозили %s[%d].", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] разаморозил Вас.", 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:weapon(playerid, const params[])
{
    #define MAX_PLAYER_WEAPON_SLOT    13

    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /weapon [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    new 
        string[(46 + (- 2 + 32) + (- 2 + 4) + (- 2 + 2)) * MAX_PLAYER_WEAPON_SLOT + 1],
        header_string[36 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1],
        weapon_name[32],
        weapon[MAX_PLAYER_WEAPON_SLOT],
        ammo[MAX_PLAYER_WEAPON_SLOT],
        bool: is_found = false;
    
    format(header_string, sizeof header_string, "{FFFFFF}Информация об оружии: %s[%d]",
            GetPlayerNameEx(targetid),
            targetid
    );

    for (new i = 0; i < MAX_PLAYER_WEAPON_SLOT; ++i) {
        GetPlayerWeaponData(targetid, i, weapon[i], ammo[i]);

        if (!weapon[i]) {
            continue;
        }

        GetWeaponName(weapon[i], weapon_name, sizeof weapon_name);

        format(string, sizeof string, "%s{FFFFFF}- Оружие: %s (Патронов: %d) | Слот: %d\n",
                string,
                weapon_name,
                ammo[i],
                i
        );

        is_found = true;
    }

    if (!is_found) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У данного игрока отсутствует оружие.");
    }

    ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_WEAPON_INFO, 
        DIALOG_STYLE_MSGBOX,
		header_string,
		string,
		!"Закрыть", !""
	);

    #undef MAX_PLAYER_WEAPON_SLOT
    return 1;
}

CMD:veh(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new 
        modelid, 
        color1,
        color2;

    if (sscanf(params, !"ddd", modelid, color1, color2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"veh: /repair_veh [Модель] [Цвет 1] [Цвет 2]");
    }

    if (!(400 <= modelid <= 611)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Модель создаваемого транспорта должна быть от 400 до 600.");
    }

    if (!(0 <= color1 <= 255)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Первый цвет создаваемого транспорта должен быть от 0 до 255.");
    }

    if (!(0 <= color2 <= 255)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Второй цвет создаваемого транспорта должен быть от 0 до 255.");
    }

    if (GetPlayerInterior(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, "Вы не можете создать транспорт в интерьере.");
    }

    new
        string[50 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) * 3 + 1],
        vehicleid,
	    Float: x,
	    Float: y,
	    Float: z,
        Float: angle;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);

    vehicleid = CreateVehicle(modelid, x, y, z, angle, color1, color2, -1);

    SetVehicleVirtualWorld(vehicleid, GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(vehicleid, GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, vehicleid, 0);

    format(string, sizeof string, "Вы успешно создали транспорт ID: [%d] с цветами [%d/%d].", 
            vehicleid, 
            color1, 
            color2
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

    format(string, sizeof string, "%s[%d] создал транспорт ID: [%d] с цветами [%d/%d].",
            GetPlayerNameEx(playerid),
            playerid,
            vehicleid, 
            color1, 
            color2
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    return 1;
}

CMD:spawn_veh(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new vehicleid;

    if (sscanf(params, !"d", vehicleid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /spawn_veh [ID транспорта]");
    }

    if (GetVehicleModel(vehicleid) <= 0) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Указанный ID транспорта не найден.");
    }

    new string[42 + (- 2 + 3) + 1];

    SetVehicleToRespawn(vehicleid);

    format(string, sizeof string, "Вы успешно засппавнили транспорт ID: [%d].", 
            vehicleid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);
    
    return 1;
}

CMD:repair_veh(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new vehicleid;

    if (sscanf(params, !"d", vehicleid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /repair_veh [ID транспорта]");
    }

    if (GetVehicleModel(vehicleid) <= 0) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Указанный ID транспорта не найден.");
    }

    new string[46 + (- 2 + 3) + 1];

    RepairVehicle(vehicleid);

    format(string, sizeof string, "Вы успешно отремонтировали транспорт ID: [%d].", 
            vehicleid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);
    
    return 1;
}

CMD:goto_veh(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new vehicleid;

    if (sscanf(params, !"d", vehicleid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /goto_veh [ID транспорта]");
    }

    if (GetVehicleModel(vehicleid) <= 0) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Указанный ID транспорта не найден.");
    }

    new
        string[51 + (- 2 + 3) + 1],
        Float: x, 
        Float: y, 
        Float: z;

    GetVehiclePos(vehicleid, x, y, z);

    SetPlayerPos(playerid, x, y, z + 2.5);
    SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(vehicleid));

    format(string, sizeof string, "Вы успешно телепортировались к транспорту ID: [%d].", 
            vehicleid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:set_skin(playerid, const params[])
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }
    
    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new
        targetid = INVALID_PLAYER_ID,
        modelid;

	if (sscanf(params, !"ud", targetid, modelid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_skin [ID (1-311)]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }
    
    if (!(0 <= modelid <= 73) && !(75 <= modelid <= 311)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR,  !"Допустимые значения для модели: 0-73, 75-311");
    }
		    
	SetPlayerSkin(targetid, modelid);

	new string[46 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + (- 2 + 3) + 1];

    format(string, sizeof string, "%s[%d] выдал временный скин [%d] игроку %s[%d].",
            GetPlayerNameEx(playerid),
            playerid,
            modelid,
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

	format(string, sizeof string, "%s[%d] выдал Вам временный скин [%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            modelid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

	return 1;
}

CMD:set_score(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new
        targetid = INVALID_PLAYER_ID,
        score;

	if (sscanf(params, !"ud", targetid, score)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_score [ID игрока / часть NickName] [Очки]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    SetPlayerScore(targetid, score);

    new string[56 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] количество очков (уровень) на [%d].", 
            GetPlayerNameEx(targetid),
            targetid,
            score
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам [%d] очков (уровень).", 
            GetPlayerNameEx(playerid),
            playerid,
            score
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:set_health(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new
        targetid = INVALID_PLAYER_ID,
        Float: health;

	if (sscanf(params, !"uf", targetid, health)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_health [ID игрока / часть NickName] [Здоровье]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    SetPlayerHealth(targetid, health);

    new string[54 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] здоровье на [%.1f].", 
            GetPlayerNameEx(targetid),
            targetid,
            health
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам здоровье на [%.1f].", 
            GetPlayerNameEx(playerid),
            playerid,
            health
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:set_armour(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new
        targetid = INVALID_PLAYER_ID,
        Float: armour;

	if (sscanf(params, !"uf", targetid, armour)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_armour [ID игрока / часть NickName] [Броня]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    SetPlayerArmour(targetid, armour);

    new string[51 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] броню на [%.1f].", 
            GetPlayerNameEx(targetid),
            targetid,
            armour
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам броню на [%.1f].", 
            GetPlayerNameEx(playerid),
            playerid,
            armour
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:set_money(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_4)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new
        targetid = INVALID_PLAYER_ID,
        money;

    if (sscanf(params, !"ud", targetid, money)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_money [ID игрока / часть NickName] [Сумма]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    ResetPlayerMoney(targetid);
    GivePlayerMoney(targetid, money);

    new string[51 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы установили %s[%d] сумму денег $%d.", 
            GetPlayerNameEx(targetid),
            targetid,
            money
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] установил Вам сумму денег $%d.", 
            GetPlayerNameEx(playerid),
            playerid,
            money
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    return 1;
}

CMD:show_stats(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

	if (sscanf(params, !"u", targetid)) {        
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /show_stats [ID игрока / часть NickName]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    new
        string[500 + (- 2 + 3) * 2 + (- 2 + 7) + (- 2 + 2) * 2 + (- 2 + 1) + (- 2 + 16) + (- 2 + 3) + (- 2 + 2) + (- 2 + 3) + (- 2 + 3) * 2 + 1],
        header_string[27 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1],
        ip_adress[16],
        Float: health, 
        Float: armour;

    GetPlayerHealth(targetid, health);
    GetPlayerArmour(targetid, armour);
    GetPlayerIp(targetid, ip_adress, sizeof ip_adress);

    format(header_string, sizeof header_string, "{FFFFFF}Информация о: {BA2821}%s[%d]", 
            GetPlayerNameEx(targetid), 
            targetid
    );

    format(string, sizeof string, 
        "\
            {FFFFFF}Здоровье: {BA2821}%.1f{FFFFFF}\n\
            Броня: {BA2821}%.1f{FFFFFF}\n\
            Деньги: {BA2821}%d{FFFFFF}\n\
            Интерьер: {BA2821}%d{FFFFFF}\n\
            Виртуальный мир: {BA2821}%d{FFFFFF}\n\
            Уровень розыска: {BA2821}%d{FFFFFF}\n\
            IP-адрес:{BA2821}%s{FFFFFF}\n\
            Пинг: {BA2821}%d{FFFFFF}\n\
            Оружие: {BA2821}%d{FFFFFF}\n\
            Патроны: {BA2821}%d{FFFFFF}\n\
            Очки: {BA2821}%d{FFFFFF}\n\
            Скин: {BA2821}\
        ",
            health, 
            armour, 
            GetPlayerMoney(targetid), 
            GetPlayerInterior(targetid), 
            GetPlayerVirtualWorld(targetid), 
            GetPlayerWantedLevel(targetid), 
            ip_adress, 
            GetPlayerPing(targetid),
            GetPlayerWeapon(targetid), 
            GetPlayerAmmo(targetid), 
            GetPlayerScore(targetid), 
            GetPlayerSkin(targetid)
    );
    
    ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_SHOW_STATS, 
        DIALOG_STYLE_MSGBOX,
        header_string,
        string,
        !"Закрыть", !""
	);

    return 1;
}

CMD:aspawn(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

	if (sscanf(params, !"u", targetid)) {        
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /aspawn [ID игрока / часть NickName]");
    }

    SpawnPlayer(targetid);

    new string[51 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + 1];

    format(string, sizeof string, "Вы заспавнили %s[%d].", 
            GetPlayerNameEx(targetid),
            targetid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

	format(string, sizeof string, "Администратор %s[%d] заспавнили Вас.", 
            GetPlayerNameEx(playerid),
            playerid
    );
    SendClientMessage(targetid, HEX_COLOR_ADMIN_SUCCESS, string);

    format(string, sizeof string, "%s[%d] зареспавнил %s[%d].", 
            GetPlayerNameEx(playerid),
            playerid,
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);
    
    return 1;
}

CMD:dveh(playerid, const params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new vehicleid;

    if (sscanf(params, !"d", vehicleid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /repair_veh [ID транспорта]");
    }

    if (!GetVehicleModel(vehicleid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Некорректный ID созданного транспорта.");
    }

    DestroyVehicle(vehicleid);

    new string[35 + (- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + 1];

    format(string, sizeof string, "Вы успешно удалили транспорт ID: [%d].", 
            vehicleid
    );
    SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, string);

    format(string, sizeof string, "%s[%d] удалилил транспорт ID: [%d].",
            vehicleid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    return 1;
}

CMD:set_prefix(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }
    
    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"us[" #MAX_ADMIN_PREFIX_LENGTH "]", targetid, params)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /set_prefix [ID игрока / часть NickName] [Наименование]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX], params);

    new string[39 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + (- 2 + MAX_ADMIN_PREFIX_LENGTH) + 1];

    format(string, sizeof string, "%s[%d] установил префикс {%s}\"%s\" %s[%d].",
            GetPlayerNameEx(playerid),
            playerid,
            g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX_COLOR],
            params,
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    return 1;
}

CMD:prefix_color(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"У Вас недостаточно прав для использования команды.");
    }

    if (!admin_IsPlayerLogin(playerid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Вы не авторизованы как администратор.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"us[" #MAX_ADMIN_PREFIX_COLOR_LENGTH "]", targetid, params)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_DEFAULT, !"Используйте: /prefix_color [ID игрока / часть NickName] [Цвет]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_ERROR, !"Указанный игрок не подключён к серверу.");
    }

    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX_COLOR], params);

    new string[43 + (- 2 + MAX_PLAYER_NAME) * 2 + (- 2 + 3) * 2 + (- 2 + MAX_ADMIN_PREFIX_LENGTH) + (- 2 + MAX_ADMIN_PREFIX_COLOR_LENGTH) + 1];

    format(string, sizeof string, "%s[%d] установил префикс {%s}\"%s\" %s[%d].",
            GetPlayerNameEx(playerid),
            playerid,
            params,
            g_player_admin[playerid][E_PLAYER_ADMIN_PREFIX],
            GetPlayerNameEx(targetid),
            targetid
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

    return 1;
}