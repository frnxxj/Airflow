#include "config_system.h"
#include "config_vars.h"

#include "../includes.h"

#include "../additionals/json/json.h"
#include "../base/other/byte_arrays.h"

#include "../base/tools/cheat_info.h"

#include "features.h"

#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

typedef Json::Value json;

constexpr int CFG_XOR_KEY = 0x5EF;

__forceinline void save_uint(json& j, std::string name, unsigned int v)
{
	j[name] = v;
}

__forceinline void save_float(json& j, std::string name, float v)
{
	j[name] = v;
}

__forceinline void save_int(json& j, std::string name, int v)
{
	j[name] = v;
}

__forceinline void save_bool(json& j, std::string name, bool v)
{
	j[name] = v;
}

__forceinline void save_string(json& j, std::string name, std::string v)
{
	j[name] = v;
}

inline void save_clr(json& j, std::string name, c_float_color* v)
{
	auto& color = *v;
	j[name][xor_c("r")] = color[0];
	j[name][xor_c("g")] = color[1];
	j[name][xor_c("b")] = color[2];
	j[name][xor_c("a")] = color[3];
}

inline void save_bind(json& j, std::string name, const key_binds_t& v)
{
	j[name][xor_c("key")] = v.key;
	j[name][xor_c("type")] = v.type;
}

inline void load_uint(const json& j, std::string name, unsigned int& v)
{
	if (j[name].empty())
		return;

	v = j[name].asUInt();
}

inline void load_clr(const json& j, std::string name, c_float_color& v)
{
	if (j[name].empty())
		return;

	v[0] = j[name][xor_c("r")].asFloat();
	v[1] = j[name][xor_c("g")].asFloat();
	v[2] = j[name][xor_c("b")].asFloat();
	v[3] = j[name][xor_c("a")].asFloat();
}

inline void load_bind(json& j, std::string name, key_binds_t& v)
{
	if (j[name].empty())
		return;

	v.key = j[name][xor_c("key")].asInt();
	v.type = j[name][xor_c("type")].asInt();
}

inline void load_int(const json& j, std::string name, int& v)
{
	if (j[name].empty())
		return;

	v = j[name].asInt();
}

inline void load_bool(const json& j, std::string name, bool& v)
{
	if (j[name].empty())
		return;

	v = j[name].asBool();
}

inline void load_float(const json& j, std::string name, float& v)
{
	if (j[name].empty())
		return;

	v = j[name].asFloat();
}

inline void load_string(json& j, std::string name, char* v)
{
	if (j[name].empty())
		return;

	std::string value = j[name].asString();
	std::strcpy(v, value.data());
}

inline bool is_dir(const TCHAR* dir)
{
	uintptr_t flag = GetFileAttributesA(dir);
	if (flag == 0xFFFFFFFFUL)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
	}
	if (!(flag & FILE_ATTRIBUTE_DIRECTORY))
		return false;
	return true;
}

__forceinline std::string file_to_string(const std::string& path)
{
	std::ifstream input_file(path);
	if (!input_file.is_open())
		return "";
	return std::string((std::istreambuf_iterator< char >(input_file)), std::istreambuf_iterator< char >());
}

namespace config
{
	configs_t default_config = {};

	std::string folder = xor_c("");
	std::string sounds_folder = xor_c("");

	void create_config_folder()
	{
		MUTATION_START

		folder = xor_c("airflow\\");
		sounds_folder = xor_c("airflow\\sounds");

		if (!is_dir(folder.c_str()))
			CreateDirectoryA(folder.c_str(), NULL);

		if (!is_dir(sounds_folder.c_str()))
			CreateDirectoryA(sounds_folder.c_str(), NULL);

		if (URLDownloadToFileA(NULL,
			xor_c("https://airflow.su/client/level4444.png"),
			xor_c("csgo\\materials\\panorama\\images\\icons\\xp\\level4444.png"), BINDF_GETNEWESTVERSION, NULL) != S_OK) {
			return;
		}

		if (URLDownloadToFileA(NULL,
			xor_c("https://airflow.su/client/level25120.png"),
			xor_c("csgo\\materials\\panorama\\images\\icons\\xp\\level25120.png"), BINDF_GETNEWESTVERSION, NULL) != S_OK) {
			return;
		}

		if (URLDownloadToFileA(NULL,
			xor_c("https://airflow.su/client/level14880.png"),
			xor_c("csgo\\materials\\panorama\\images\\icons\\xp\\level14880.png"), BINDF_GETNEWESTVERSION, NULL) != S_OK) {
			return;
		}

		if (URLDownloadToFileA(NULL,
			xor_c("https://airflow.su/client/level2512.png"),
			xor_c("csgo\\materials\\panorama\\images\\icons\\xp\\level2512.png"), BINDF_GETNEWESTVERSION, NULL) != S_OK) {
			return;
		}

		if (URLDownloadToFileA(NULL,
			xor_c("https://airflow.su/client/level44440.png"),
			xor_c("csgo\\materials\\panorama\\images\\icons\\xp\\level44440.png"), BINDF_GETNEWESTVERSION, NULL) != S_OK) {
			return;
		}

		MUTATION_END
	}

	void create(const std::string& config_name)
	{
		json json_obj = {};

		auto& rage = json_obj[xor_c("rage")];
		{
			save_bool(rage, xor_c("enable"), default_config.rage.enable);
			save_bool(rage, xor_c("resolver"), default_config.rage.resolver);
			save_bool(rage, xor_c("auto_fire"), default_config.rage.auto_fire);
			save_bool(rage, xor_c("air_defensive"), default_config.rage.air_defensive);
			save_int(rage, xor_c("spike_amt"), default_config.rage.spike_amt);
			save_int(rage, xor_c("roll_amt"), default_config.rage.roll_amt);
			save_int(rage, xor_c("roll_amt_pitch"), default_config.rage.roll_amt_pitch);

#if ALPHA || _DEBUG || BETA
			save_bool(rage, xor_c("round_lagcomp"), default_config.rage.round_lagcomp);
			save_bool(rage, xor_c("delay_on_breaklc"), default_config.rage.delay_on_breaklc);
			save_bool(rage, xor_c("jitterfix"), default_config.rage.jitterfix);
			save_int(rage, xor_c("jitterfix_method"), default_config.rage.jitterfix_method);
#endif

			for (int i = 0; i < weapon_max; i++)
			{
				auto sub = xor_c("weapon_config_id ") + std::to_string(i + 1);
				save_bool(rage[sub], xor_c("enable"), default_config.rage.weapon[i].enable);
				save_bool(rage[sub], xor_c("quick_stop"), default_config.rage.weapon[i].quick_stop);
				save_bool(rage[sub], xor_c("auto_scope"), default_config.rage.weapon[i].auto_scope);
				save_bool(rage[sub], xor_c("prefer_body"), default_config.rage.weapon[i].prefer_body);
				save_bool(rage[sub], xor_c("prefer_safe"), default_config.rage.weapon[i].prefer_safe);
				save_bool(rage[sub], xor_c("strict_mode"), default_config.rage.weapon[i].strict_mode);

				save_int(rage[sub], xor_c("hitchance"), default_config.rage.weapon[i].hitchance);
				save_int(rage[sub], xor_c("mindamage"), default_config.rage.weapon[i].mindamage);
				save_int(rage[sub], xor_c("damage_override"), default_config.rage.weapon[i].damage_override);

				save_int(rage[sub], xor_c("scale_head"), default_config.rage.weapon[i].scale_head);
				save_int(rage[sub], xor_c("scale_body"), default_config.rage.weapon[i].scale_body);

				save_uint(rage[sub], xor_c("quick_stop_options"), default_config.rage.weapon[i].quick_stop_options);
				save_uint(rage[sub], xor_c("hitboxes"), default_config.rage.weapon[i].hitboxes);
			//	save_uint(rage[sub], xor_c("hitchance_skips"), default_config.rage.weapon[i].hitchance_skips);
			}
		}

		auto& legit = json_obj[xor_c("legit")];
		{
			save_bool(legit, xor_c("enable"), default_config.legit.enable);
			save_bool(legit, xor_c("autowall"), default_config.legit.autowall);
			save_bool(legit, xor_c("flash_check"), default_config.legit.flash_check);
			save_bool(legit, xor_c("smoke_check"), default_config.legit.smoke_check);

			save_int(legit, xor_c("min_damage"), default_config.legit.min_damage);

			auto& rcs_legit = legit[xor_c("rcs")];
			{
				save_bool(rcs_legit, xor_c("enable"), default_config.legit.rcs.enable);
				save_int(rcs_legit, xor_c("amount"), default_config.legit.rcs.amount);
			}

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("legit_weapon ") + std::to_string(i + 1);

				auto& legit_cfg = default_config.legit.legit_weapon[i];
				auto& legit_cfg_json = legit[sub];
				{
					save_bool(legit_cfg_json, xor_c("enable"), legit_cfg.enable);
					save_bool(legit_cfg_json, xor_c("backtrack"), legit_cfg.backtrack);
					save_bool(legit_cfg_json, xor_c("quick_stop"), legit_cfg.quick_stop);
					save_bool(legit_cfg_json, xor_c("fov_cicle"), legit_cfg.fov_cicle);

					save_int(legit_cfg_json, xor_c("fov"), legit_cfg.fov);
					save_int(legit_cfg_json, xor_c("smooth"), legit_cfg.smooth);
					save_int(legit_cfg_json, xor_c("aim_delay"), legit_cfg.aim_delay);

					save_uint(legit_cfg_json, xor_c("hitboxes"), legit_cfg.hitboxes);
					save_clr(legit_cfg_json, xor_c("circle_color"), &legit_cfg.circle_color);
				}
			}
		}

		auto& antihit = json_obj[xor_c("antihit")];
		{
			save_bool(antihit, xor_c("enable"), default_config.antihit.enable);
			save_bool(antihit, xor_c("at_targets"), default_config.antihit.at_targets);
			save_bool(antihit, xor_c("silent_onshot"), default_config.antihit.silent_onshot);
			save_bool(antihit, xor_c("def_pitch"), default_config.antihit.def_pitch);
			save_bool(antihit, xor_c("def_yaw"), default_config.antihit.def_yaw);
			save_bool(antihit, xor_c("desync"), default_config.antihit.desync);
			save_bool(antihit, xor_c("distortion"), default_config.antihit.distortion);
			save_bool(antihit, xor_c("fakelag"), default_config.antihit.fakelag);
			save_bool(antihit, xor_c("jitter_move"), default_config.antihit.jitter_move);
			save_bool(antihit, xor_c("random_dsy"), default_config.antihit.random_dsy);
			save_bool(antihit, xor_c("random_amount"), default_config.antihit.random_amount);
			save_bool(antihit, xor_c("random_jitter"), default_config.antihit.random_jitter);

			save_int(antihit, xor_c("def_aa_mode"), default_config.antihit.def_aa_mode);
			save_int(antihit, xor_c("pitch"), default_config.antihit.pitch);
			save_int(antihit, xor_c("yaw"), default_config.antihit.yaw);
			save_int(antihit, xor_c("yaw_add"), default_config.antihit.yaw_add);
			save_int(antihit, xor_c("desync_mode"), default_config.antihit.desync_mode);
			save_int(antihit, xor_c("jitter_mode"), default_config.antihit.jitter_mode);
			save_int(antihit, xor_c("jitter_range"), default_config.antihit.jitter_range);
			save_int(antihit, xor_c("desync_left"), default_config.antihit.desync_left);
			save_int(antihit, xor_c("desync_right"), default_config.antihit.desync_right);
			save_int(antihit, xor_c("distortion_range"), default_config.antihit.distortion_range);
			save_int(antihit, xor_c("distortion_pitch"), default_config.antihit.distortion_pitch);
			save_int(antihit, xor_c("fakelag_limit"), default_config.antihit.fakelag_limit);
			save_uint(antihit, xor_c("fakelag_conditions"), default_config.antihit.fakelag_conditions);
		}

		auto& visuals = json_obj[xor_c("visuals")];
		{
			for (int i = 0; i < esp_max; ++i)
			{
				auto sub = xor_c("esp ") + std::to_string(i + 1);

				auto& esp_cfg = default_config.visuals.esp[i];
				auto& esp_cfg_json = visuals[sub];
				{
					save_bool(esp_cfg_json, xor_c("enable"), esp_cfg.enable);
					save_bool(esp_cfg_json, xor_c("nade_offscreen"), esp_cfg.nade_offscreen);
					save_int(esp_cfg_json, xor_c("offscreen_dist"), esp_cfg.offscreen_dist);
					save_int(esp_cfg_json, xor_c("offscreen_size"), esp_cfg.offscreen_size);
					save_uint(esp_cfg_json, xor_c("elements"), esp_cfg.elements);

					auto& esp_colors = esp_cfg_json[xor_c("colors")];
					{
						save_clr(esp_colors, xor_c("box"), &esp_cfg.colors.box);
						save_clr(esp_colors, xor_c("name"), &esp_cfg.colors.name);
						save_clr(esp_colors, xor_c("health"), &esp_cfg.colors.health);
						save_clr(esp_colors, xor_c("weapon"), &esp_cfg.colors.weapon);
						save_clr(esp_colors, xor_c("ammo_bar"), &esp_cfg.colors.ammo_bar);
						save_clr(esp_colors, xor_c("offscreen_arrow"), &esp_cfg.colors.offscreen_arrow);
						save_clr(esp_colors, xor_c("offscreen_arrow_outline"), &esp_cfg.colors.offscreen_arrow_outline);
						save_clr(esp_colors, xor_c("glow"), &esp_cfg.colors.glow);
						save_clr(esp_colors, xor_c("skeleton"), &esp_cfg.colors.skeleton);
						save_clr(esp_colors, xor_c("molotov_range"), &esp_cfg.colors.molotov_range);
						save_clr(esp_colors, xor_c("smoke_range"), &esp_cfg.colors.smoke_range);
					}
				}
			}

			save_bool(visuals, xor_c("grenade_predict"), default_config.visuals.grenade_predict);
			save_clr(visuals, xor_c("predict_clr"), &default_config.visuals.predict_clr);
			save_bool(visuals, xor_c("show_all_history"), &default_config.visuals.show_all_history);

			save_bool(visuals, xor_c("local_glow"), default_config.visuals.local_glow);
			save_clr(visuals, xor_c("local_glow_color"), &default_config.visuals.local_glow_color);

			save_bool(visuals, xor_c("grenade_warning"), default_config.visuals.grenade_warning);
			save_bool(visuals, xor_c("grenade_warning_line"), default_config.visuals.grenade_warning_line);
			save_clr(visuals, xor_c("warning_clr"), &default_config.visuals.warning_clr);

			save_bool(visuals[xor_c("eventlog")], xor_c("enable"), default_config.visuals.eventlog.enable);
			save_bool(visuals[xor_c("eventlog")], xor_c("filter_console"), default_config.visuals.eventlog.filter_console);
			save_uint(visuals[xor_c("eventlog")], xor_c("logs"), default_config.visuals.eventlog.logs);

			for (int i = 0; i < c_max; ++i)
			{
				auto sub = xor_c("chams ") + std::to_string(i + 1);
				auto& chams_sub = visuals[sub];
				{
					save_bool(chams_sub, xor_c("enable"), default_config.visuals.chams[i].enable);
					save_int(chams_sub, xor_c("material"), default_config.visuals.chams[i].material);
					save_int(chams_sub, xor_c("glow_fill"), default_config.visuals.chams[i].glow_fill);
					save_int(chams_sub, xor_c("shot_duration"), default_config.visuals.chams[i].shot_duration);
					save_clr(chams_sub, xor_c("main_color"), &default_config.visuals.chams[i].main_color);
					save_clr(chams_sub, xor_c("glow_color"), &default_config.visuals.chams[i].glow_color);
					save_string(chams_sub, xor_c("chams_sprite"), default_config.visuals.chams[i].chams_sprite);
				}
			}
		}

		auto& misc = json_obj[xor_c("misc")];
		{
			save_bool(misc, xor_c("auto_jump"), default_config.misc.auto_jump);
			save_bool(misc, xor_c("auto_strafe"), default_config.misc.auto_strafe);
			save_bool(misc, xor_c("fast_stop"), default_config.misc.fast_stop);
			save_bool(misc, xor_c("slide_walk"), default_config.misc.slide_walk);
			save_bool(misc, xor_c("pen_xhair"), default_config.misc.pen_xhair);

			for (int i = 0; i < 3; i++)
			{
				save_int(misc[xor_c("fov") + std::to_string(i + 1)], xor_c("amt_"), default_config.misc.fovs[i]);
				save_int(misc[xor_c("viewmodel_pos") + std::to_string(i + 1)], xor_c("pos_"), default_config.misc.viewmodel_pos[i]);
			}

			save_int(misc, xor_c("aspect_ratio"), default_config.misc.aspect_ratio);

			save_bool(misc, xor_c("thirdperson_dead"), default_config.misc.thirdperson_dead);
			save_int(misc, xor_c("thirdperson_dist"), default_config.misc.thirdperson_dist);

			save_bool(misc, xor_c("impacts"), default_config.misc.impacts);
			save_clr(misc, xor_c("server_clr"), &default_config.misc.server_clr);
			save_clr(misc, xor_c("client_clr"), &default_config.misc.client_clr);
			save_int(misc, xor_c("impact_size"), default_config.misc.impact_size);

			save_uint(misc, xor_c("tracers"), default_config.misc.tracers);
			save_int(misc, xor_c("tracer_type"), default_config.misc.tracer_type);

			for (int i = 0; i < 3; i++)
				save_clr(misc[xor_c("trace_clr") + std::to_string(i + 1)], xor_c("clr_"), &default_config.misc.trace_clr[i]);

			for (int i = 0; i < world_clr_max; i++)
				save_clr(misc[xor_c("world_clr") + std::to_string(i + 1)], xor_c("clr_"), &default_config.misc.world_clr[i]);

			save_uint(misc, xor_c("hitmarker"), default_config.misc.hitmarker);
			save_clr(misc, xor_c("hitmarker_clr"), &default_config.misc.hitmarker_clr);
			save_bool(misc, xor_c("damage"), default_config.misc.damage);
			save_clr(misc, xor_c("damage_clr"), &default_config.misc.damage_clr);
			save_int(misc, xor_c("sound"), default_config.misc.sound);
			save_int(misc, xor_c("sound_volume"), default_config.misc.sound_volume);
			save_bool(misc, xor_c("unlock_inventory"), default_config.misc.unlock_inventory);
			save_bool(misc, xor_c("clantag"), default_config.misc.clantag);
			save_bool(misc, xor_c("spoof_sv_cheats"), default_config.misc.spoof_sv_cheats);
			save_bool(misc, xor_c("unlock_hidden_cvars"), default_config.misc.unlock_hidden_cvars);
			save_bool(misc, xor_c("bypass_sv_pure"), default_config.misc.bypass_sv_pure);
			save_bool(misc, xor_c("remove_ads"), default_config.misc.remove_ads);
			save_bool(misc, xor_c("snip_crosshair"), default_config.misc.snip_crosshair);
			save_bool(misc, xor_c("preverse_killfeed"), default_config.misc.preverse_killfeed);
			save_bool(misc, xor_c("force_radar"), default_config.misc.force_radar);
			save_int(misc, xor_c("ragdoll_gravity"), default_config.misc.ragdoll_gravity);
			save_int(misc, xor_c("strafe_smooth"), default_config.misc.strafe_smooth);

			save_bool(misc[xor_c("buybot")], xor_c("enable"), default_config.misc.buybot.enable);
			save_int(misc[xor_c("buybot")], xor_c("main_weapon"), default_config.misc.buybot.main_weapon);
			save_int(misc[xor_c("buybot")], xor_c("second_weapon"), default_config.misc.buybot.second_weapon);
			save_uint(misc[xor_c("buybot")], xor_c("other_items"), default_config.misc.buybot.other_items);

			save_uint(misc, xor_c("removals"), default_config.misc.removals);
			save_uint(misc, xor_c("world_modulation"), default_config.misc.world_modulation);

			save_bool(misc, xor_c("blend_scope"), default_config.misc.blend_scope);
			save_int(misc, xor_c("scope_amt"), default_config.misc.scope_amt);
			save_int(misc, xor_c("attachments_amt"), default_config.misc.attachments_amt);

			save_int(misc, xor_c("bloom_scale"), default_config.misc.bloom_scale);
			save_bool(misc, xor_c("custom_bloom"), default_config.misc.custom_bloom);
			save_bool(misc, xor_c("retrack_peek"), default_config.misc.retrack_peek);
			save_bool(misc, xor_c("viewmodel_scope"), default_config.misc.viewmodel_scope);
			save_bool(misc, xor_c("fix_sensitivity"), default_config.misc.fix_sensitivity);
			save_bool(misc, xor_c("skip_second_zoom"), default_config.misc.skip_second_zoom);
			save_int(misc, xor_c("exposure_min"), default_config.misc.exposure_min);
			save_int(misc, xor_c("exposure_max"), default_config.misc.exposure_max);
			save_clr(misc, xor_c("accent_color"), &default_config.misc.ui_color);
			save_uint(misc, xor_c("menu_indicators"), default_config.misc.menu_indicators);
			save_uint(misc, xor_c("animation_changes"), default_config.misc.animation_changes);

			save_bool(misc, xor_c("custom_fog"), default_config.misc.custom_fog);
			save_int(misc, xor_c("fog_start"), default_config.misc.fog_start);
			save_int(misc, xor_c("fog_end"), default_config.misc.fog_end);
			save_int(misc, xor_c("fog_density"), default_config.misc.fog_density);
			save_int(misc, xor_c("skybox"), default_config.misc.skybox);
			save_string(misc, xor_c("skybox_name"), default_config.misc.skybox_name);
			save_string(misc, xor_c("sound_name"), default_config.misc.sound_name);
			save_int(misc, xor_c("prop_alpha"), default_config.misc.prop_alpha);
			save_uint(misc, xor_c("world_material_options"), default_config.misc.world_material_options);

			save_float(misc[xor_c("keybind_pos")], xor_c("x"), default_config.misc.keybind_position.x);
			save_float(misc[xor_c("keybind_pos")], xor_c("y"), default_config.misc.keybind_position.y);

			save_float(misc[xor_c("spectator_pos")], xor_c("x"), default_config.misc.spectators_position.x);
			save_float(misc[xor_c("spectator_pos")], xor_c("y"), default_config.misc.spectators_position.y);

			save_float(misc[xor_c("bomb_pos")], xor_c("x"), default_config.misc.bomb_position.x);
			save_float(misc[xor_c("bomb_pos")], xor_c("y"), default_config.misc.bomb_position.y);

			save_int(misc[xor_c("sunset_angle")], xor_c("x"), default_config.misc.sunset_angle.x);
			save_int(misc[xor_c("sunset_angle")], xor_c("y"), default_config.misc.sunset_angle.y);
			save_int(misc[xor_c("sunset_angle")], xor_c("z"), default_config.misc.sunset_angle.y);

			save_clr(misc, xor_c("autopeek_clr"), &default_config.misc.autopeek_clr);
			save_clr(misc, xor_c("autopeek_clr_back"), &default_config.misc.autopeek_clr_back);
			save_clr(misc, xor_c("texture_reflect_clr"), &default_config.misc.texture_reflect_clr);
		}

		auto& skins = json_obj[xor_c("skins")];
		{
			save_int(skins, xor_c("model_glove"), default_config.skins.model_glove);
			save_int(skins, xor_c("glove_skin"), default_config.skins.glove_skin);
			save_string(skins, xor_c("custom_model_t"), default_config.skins.custom_model_t);
			save_string(skins, xor_c("custom_model_ct"), default_config.skins.custom_model_ct);
			save_int(skins, xor_c("masks"), default_config.skins.masks);

			save_int(skins[xor_c("agents")], xor_c("ct"), default_config.skins.model_ct);
			save_int(skins[xor_c("agents")], xor_c("t"), default_config.skins.model_t);

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("skin_weapon ") + std::to_string(i + 1);

				auto& skins_cfg = default_config.skins.skin_weapon[i];
				auto& skins_cfg_json = skins[sub];
				{
					save_bool(skins_cfg_json, xor_c("enable"), skins_cfg.enable);
					save_int(skins_cfg_json, xor_c("knife_model"), skins_cfg.knife_model);
					save_int(skins_cfg_json, xor_c("skin"), skins_cfg.skin);
				}
			}
		}

		auto& binds = json_obj[xor_c("binds")];
		{
			for (int i = 0; i < binds_max; i++)
			{
				auto add = std::to_string(i + 1);
				save_bind(binds, add, default_config.binds[i]);
			}
		}

		std::string file = folder + config_name;
		auto str = json_obj.toStyledString();
		for (int i = 0; i < str.size(); ++i)
			str[i] ^= CFG_XOR_KEY;

		std::ofstream file_out(file);
		if (file_out.good())
			file_out << str;
		file_out.close();
	}

	void erase(const std::string& config_name)
	{
		std::string file = folder + config_name;
		remove(file.c_str());
	}

	void save(const std::string& config_name)
	{
		json json_obj = {};

		auto& rage = json_obj[xor_c("rage")];
		{
			save_bool(rage, xor_c("enable"), g_cfg.rage.enable);
			save_bool(rage, xor_c("resolver"), g_cfg.rage.resolver);
			save_bool(rage, xor_c("auto_fire"), g_cfg.rage.auto_fire);
			save_bool(rage, xor_c("air_defensive"), g_cfg.rage.air_defensive);
			save_int(rage, xor_c("spike_amt"), g_cfg.rage.spike_amt);
			save_int(rage, xor_c("roll_amt"), g_cfg.rage.roll_amt);
			save_int(rage, xor_c("roll_amt_pitch"), g_cfg.rage.roll_amt_pitch);

#if ALPHA || _DEBUG || BETA
			save_bool(rage, xor_c("round_lagcomp"), g_cfg.rage.round_lagcomp);
			save_bool(rage, xor_c("delay_on_breaklc"), g_cfg.rage.delay_on_breaklc);
			save_bool(rage, xor_c("jitterfix"), g_cfg.rage.jitterfix);
			save_int(rage, xor_c("jitterfix_method"), g_cfg.rage.jitterfix_method);
#endif

			for (int i = 0; i < weapon_max; i++)
			{
				auto sub = xor_c("weapon_config_id ") + std::to_string(i + 1);
				save_bool(rage[sub], xor_c("enable"), g_cfg.rage.weapon[i].enable);
				save_bool(rage[sub], xor_c("quick_stop"), g_cfg.rage.weapon[i].quick_stop);
				save_bool(rage[sub], xor_c("auto_scope"), g_cfg.rage.weapon[i].auto_scope);
				save_bool(rage[sub], xor_c("prefer_body"), g_cfg.rage.weapon[i].prefer_body);
				save_bool(rage[sub], xor_c("prefer_safe"), g_cfg.rage.weapon[i].prefer_safe);
				save_bool(rage[sub], xor_c("strict_mode"), g_cfg.rage.weapon[i].strict_mode);

				save_int(rage[sub], xor_c("hitchance"), g_cfg.rage.weapon[i].hitchance);
				save_int(rage[sub], xor_c("mindamage"), g_cfg.rage.weapon[i].mindamage);
				save_int(rage[sub], xor_c("damage_override"), g_cfg.rage.weapon[i].damage_override);

				save_int(rage[sub], xor_c("scale_head"), g_cfg.rage.weapon[i].scale_head);
				save_int(rage[sub], xor_c("scale_body"), g_cfg.rage.weapon[i].scale_body);

				save_uint(rage[sub], xor_c("quick_stop_options"), g_cfg.rage.weapon[i].quick_stop_options);
				save_uint(rage[sub], xor_c("hitboxes"), g_cfg.rage.weapon[i].hitboxes);
				//save_uint(rage[sub], xor_c("hitchance_skips"), g_cfg.rage.weapon[i].hitchance_skips);
			}
		}

		auto& legit = json_obj[xor_c("legit")];
		{
			save_bool(legit, xor_c("enable"), g_cfg.legit.enable);
			save_bool(legit, xor_c("autowall"), g_cfg.legit.autowall);
			save_bool(legit, xor_c("flash_check"), g_cfg.legit.flash_check);
			save_bool(legit, xor_c("smoke_check"), g_cfg.legit.smoke_check);

			save_int(legit, xor_c("min_damage"), g_cfg.legit.min_damage);

			auto& rcs_legit = legit[xor_c("rcs")];
			{
				save_bool(rcs_legit, xor_c("enable"), g_cfg.legit.rcs.enable);
				save_int(rcs_legit, xor_c("amount"), g_cfg.legit.rcs.amount);
			}

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("legit_weapon ") + std::to_string(i + 1);

				auto& legit_cfg = g_cfg.legit.legit_weapon[i];
				auto& legit_cfg_json = legit[sub];
				{
					save_bool(legit_cfg_json, xor_c("enable"), legit_cfg.enable);
					save_bool(legit_cfg_json, xor_c("backtrack"), legit_cfg.backtrack);
					save_bool(legit_cfg_json, xor_c("quick_stop"), legit_cfg.quick_stop);
					save_bool(legit_cfg_json, xor_c("fov_cicle"), legit_cfg.fov_cicle);

					save_int(legit_cfg_json, xor_c("fov"), legit_cfg.fov);
					save_int(legit_cfg_json, xor_c("smooth"), legit_cfg.smooth);
					save_int(legit_cfg_json, xor_c("aim_delay"), legit_cfg.aim_delay);

					save_uint(legit_cfg_json, xor_c("hitboxes"), legit_cfg.hitboxes);
					save_clr(legit_cfg_json, xor_c("circle_color"), &legit_cfg.circle_color);
				}
			}
		}

		auto& antihit = json_obj[xor_c("antihit")];
		{
			save_bool(antihit, xor_c("enable"), g_cfg.antihit.enable);
			save_bool(antihit, xor_c("at_targets"), g_cfg.antihit.at_targets);
			save_bool(antihit, xor_c("silent_onshot"), g_cfg.antihit.silent_onshot);
			save_bool(antihit, xor_c("def_pitch"), g_cfg.antihit.def_pitch);
			save_bool(antihit, xor_c("def_yaw"), g_cfg.antihit.def_yaw);
			save_bool(antihit, xor_c("desync"), g_cfg.antihit.desync);
			save_bool(antihit, xor_c("distortion"), g_cfg.antihit.distortion);
			save_bool(antihit, xor_c("fakelag"), g_cfg.antihit.fakelag);
			save_bool(antihit, xor_c("jitter_move"), g_cfg.antihit.jitter_move);
			save_bool(antihit, xor_c("random_dsy"), g_cfg.antihit.random_dsy);
			save_bool(antihit, xor_c("random_amount"), g_cfg.antihit.random_amount);
			save_bool(antihit, xor_c("random_jitter"), g_cfg.antihit.random_jitter);

			save_int(antihit, xor_c("def_aa_mode"), g_cfg.antihit.def_aa_mode);
			save_int(antihit, xor_c("pitch"), g_cfg.antihit.pitch);
			save_int(antihit, xor_c("yaw"), g_cfg.antihit.yaw);
			save_int(antihit, xor_c("yaw_add"), g_cfg.antihit.yaw_add);
			save_int(antihit, xor_c("desync_mode"), g_cfg.antihit.desync_mode);
			save_int(antihit, xor_c("jitter_mode"), g_cfg.antihit.jitter_mode);
			save_int(antihit, xor_c("jitter_range"), g_cfg.antihit.jitter_range);
			save_int(antihit, xor_c("desync_left"), g_cfg.antihit.desync_left);
			save_int(antihit, xor_c("desync_right"), g_cfg.antihit.desync_right);
			save_int(antihit, xor_c("distortion_range"), g_cfg.antihit.distortion_range);
			save_int(antihit, xor_c("distortion_pitch"), g_cfg.antihit.distortion_pitch);
			save_int(antihit, xor_c("fakelag_limit"), g_cfg.antihit.fakelag_limit);
			save_uint(antihit, xor_c("fakelag_conditions"), g_cfg.antihit.fakelag_conditions);
		}

		auto& visuals = json_obj[xor_c("visuals")];
		{
			for (int i = 0; i < esp_max; ++i)
			{
				auto sub = xor_c("esp ") + std::to_string(i + 1);

				auto& esp_cfg = g_cfg.visuals.esp[i];
				auto& esp_cfg_json = visuals[sub];
				{
					save_bool(esp_cfg_json, xor_c("enable"), esp_cfg.enable);
					save_bool(esp_cfg_json, xor_c("nade_offscreen"), esp_cfg.nade_offscreen);
					save_int(esp_cfg_json, xor_c("offscreen_dist"), esp_cfg.offscreen_dist);
					save_int(esp_cfg_json, xor_c("offscreen_size"), esp_cfg.offscreen_size);
					save_uint(esp_cfg_json, xor_c("elements"), esp_cfg.elements);

					auto& esp_colors = esp_cfg_json[xor_c("colors")];
					{
						save_clr(esp_colors, xor_c("box"), &esp_cfg.colors.box);
						save_clr(esp_colors, xor_c("name"), &esp_cfg.colors.name);
						save_clr(esp_colors, xor_c("health"), &esp_cfg.colors.health);
						save_clr(esp_colors, xor_c("weapon"), &esp_cfg.colors.weapon);
						save_clr(esp_colors, xor_c("ammo_bar"), &esp_cfg.colors.ammo_bar);
						save_clr(esp_colors, xor_c("offscreen_arrow"), &esp_cfg.colors.offscreen_arrow);
						save_clr(esp_colors, xor_c("offscreen_arrow_outline"), &esp_cfg.colors.offscreen_arrow_outline);
						save_clr(esp_colors, xor_c("glow"), &esp_cfg.colors.glow);
						save_clr(esp_colors, xor_c("skeleton"), &esp_cfg.colors.skeleton);
						save_clr(esp_colors, xor_c("molotov_range"), &esp_cfg.colors.molotov_range);
						save_clr(esp_colors, xor_c("smoke_range"), &esp_cfg.colors.smoke_range);
					}
				}
			}

			save_bool(visuals, xor_c("grenade_predict"), g_cfg.visuals.grenade_predict);
			save_clr(visuals, xor_c("predict_clr"), &g_cfg.visuals.predict_clr);
			save_bool(visuals, xor_c("show_all_history"), &g_cfg.visuals.show_all_history);

			save_bool(visuals, xor_c("local_glow"), g_cfg.visuals.local_glow);
			save_clr(visuals, xor_c("local_glow_color"), &g_cfg.visuals.local_glow_color);

			save_bool(visuals, xor_c("grenade_warning"), g_cfg.visuals.grenade_warning);
			save_bool(visuals, xor_c("grenade_warning_line"), g_cfg.visuals.grenade_warning_line);
			save_clr(visuals, xor_c("warning_clr"), &g_cfg.visuals.warning_clr);

			save_bool(visuals[xor_c("eventlog")], xor_c("enable"), g_cfg.visuals.eventlog.enable);
			save_bool(visuals[xor_c("eventlog")], xor_c("filter_console"), g_cfg.visuals.eventlog.filter_console);
			save_uint(visuals[xor_c("eventlog")], xor_c("logs"), g_cfg.visuals.eventlog.logs);

			for (int i = 0; i < c_max; ++i)
			{
				auto sub = xor_c("chams ") + std::to_string(i + 1);
				auto& chams_sub = visuals[sub];
				{
					save_bool(chams_sub, xor_c("enable"), g_cfg.visuals.chams[i].enable);
					save_int(chams_sub, xor_c("material"), g_cfg.visuals.chams[i].material);
					save_int(chams_sub, xor_c("glow_fill"), g_cfg.visuals.chams[i].glow_fill);
					save_int(chams_sub, xor_c("shot_duration"), g_cfg.visuals.chams[i].shot_duration);
					save_clr(chams_sub, xor_c("main_color"), &g_cfg.visuals.chams[i].main_color);
					save_clr(chams_sub, xor_c("glow_color"), &g_cfg.visuals.chams[i].glow_color);
					save_string(chams_sub, xor_c("chams_sprite"), g_cfg.visuals.chams[i].chams_sprite);
				}
			}
		}

		auto& misc = json_obj[xor_c("misc")];
		{
			save_bool(misc, xor_c("auto_jump"), g_cfg.misc.auto_jump);
			save_bool(misc, xor_c("auto_strafe"), g_cfg.misc.auto_strafe);
			save_bool(misc, xor_c("fast_stop"), g_cfg.misc.fast_stop);
			save_bool(misc, xor_c("slide_walk"), g_cfg.misc.slide_walk);
			save_bool(misc, xor_c("pen_xhair"), g_cfg.misc.pen_xhair);

			for (int i = 0; i < 3; i++)
			{
				save_int(misc[xor_c("fov") + std::to_string(i + 1)], xor_c("amt_"), g_cfg.misc.fovs[i]);
				save_int(misc[xor_c("viewmodel_pos") + std::to_string(i + 1)], xor_c("pos_"), g_cfg.misc.viewmodel_pos[i]);
			}

			save_int(misc, xor_c("aspect_ratio"), g_cfg.misc.aspect_ratio);

			save_bool(misc, xor_c("thirdperson_dead"), g_cfg.misc.thirdperson_dead);
			save_int(misc, xor_c("thirdperson_dist"), g_cfg.misc.thirdperson_dist);

			save_bool(misc, xor_c("impacts"), g_cfg.misc.impacts);
			save_clr(misc, xor_c("server_clr"), &g_cfg.misc.server_clr);
			save_clr(misc, xor_c("client_clr"), &g_cfg.misc.client_clr);
			save_int(misc, xor_c("impact_size"), g_cfg.misc.impact_size);

			save_uint(misc, xor_c("tracers"), g_cfg.misc.tracers);
			save_int(misc, xor_c("tracer_type"), g_cfg.misc.tracer_type);

			for (int i = 0; i < 3; i++)
				save_clr(misc[xor_c("trace_clr") + std::to_string(i + 1)], xor_c("clr_"), &g_cfg.misc.trace_clr[i]);

			for (int i = 0; i < world_clr_max; i++)
				save_clr(misc[xor_c("world_clr") + std::to_string(i + 1)], xor_c("clr_"), &g_cfg.misc.world_clr[i]);

			save_uint(misc, xor_c("hitmarker"), g_cfg.misc.hitmarker);
			save_clr(misc, xor_c("hitmarker_clr"), &g_cfg.misc.hitmarker_clr);
			save_bool(misc, xor_c("damage"), g_cfg.misc.damage);
			save_clr(misc, xor_c("damage_clr"), &g_cfg.misc.damage_clr);
			save_int(misc, xor_c("sound"), g_cfg.misc.sound);
			save_int(misc, xor_c("sound_volume"), g_cfg.misc.sound_volume);
			save_bool(misc, xor_c("unlock_inventory"), g_cfg.misc.unlock_inventory);
			save_bool(misc, xor_c("clantag"), g_cfg.misc.clantag);
			save_bool(misc, xor_c("spoof_sv_cheats"), g_cfg.misc.spoof_sv_cheats);
			save_bool(misc, xor_c("unlock_hidden_cvars"), g_cfg.misc.unlock_hidden_cvars);
			save_bool(misc, xor_c("bypass_sv_pure"), g_cfg.misc.bypass_sv_pure);
			save_bool(misc, xor_c("remove_ads"), g_cfg.misc.remove_ads);
			save_bool(misc, xor_c("snip_crosshair"), g_cfg.misc.snip_crosshair);
			save_bool(misc, xor_c("preverse_killfeed"), g_cfg.misc.preverse_killfeed);
			save_bool(misc, xor_c("force_radar"), g_cfg.misc.force_radar);
			save_int(misc, xor_c("ragdoll_gravity"), g_cfg.misc.ragdoll_gravity);
			save_int(misc, xor_c("strafe_smooth"), g_cfg.misc.strafe_smooth);

			save_bool(misc[xor_c("buybot")], xor_c("enable"), g_cfg.misc.buybot.enable);
			save_int(misc[xor_c("buybot")], xor_c("main_weapon"), g_cfg.misc.buybot.main_weapon);
			save_int(misc[xor_c("buybot")], xor_c("second_weapon"), g_cfg.misc.buybot.second_weapon);
			save_uint(misc[xor_c("buybot")], xor_c("other_items"), g_cfg.misc.buybot.other_items);

			save_uint(misc, xor_c("removals"), g_cfg.misc.removals);
			save_uint(misc, xor_c("world_modulation"), g_cfg.misc.world_modulation);

			save_bool(misc, xor_c("blend_scope"), g_cfg.misc.blend_scope);
			save_int(misc, xor_c("scope_amt"), g_cfg.misc.scope_amt);
			save_int(misc, xor_c("attachments_amt"), g_cfg.misc.attachments_amt);

			save_int(misc, xor_c("bloom_scale"), g_cfg.misc.bloom_scale);
			save_bool(misc, xor_c("custom_bloom"), g_cfg.misc.custom_bloom);
			save_bool(misc, xor_c("retrack_peek"), g_cfg.misc.retrack_peek);
			save_bool(misc, xor_c("viewmodel_scope"), g_cfg.misc.viewmodel_scope);
			save_bool(misc, xor_c("fix_sensitivity"), g_cfg.misc.fix_sensitivity);
			save_bool(misc, xor_c("skip_second_zoom"), g_cfg.misc.skip_second_zoom);
			save_int(misc, xor_c("exposure_min"), g_cfg.misc.exposure_min);
			save_int(misc, xor_c("exposure_max"), g_cfg.misc.exposure_max);
			save_clr(misc, xor_c("accent_color"), &g_cfg.misc.ui_color);
			save_uint(misc, xor_c("menu_indicators"), g_cfg.misc.menu_indicators);
			save_uint(misc, xor_c("animation_changes"), g_cfg.misc.animation_changes);

			save_bool(misc, xor_c("custom_fog"), g_cfg.misc.custom_fog);
			save_int(misc, xor_c("fog_start"), g_cfg.misc.fog_start);
			save_int(misc, xor_c("fog_end"), g_cfg.misc.fog_end);
			save_int(misc, xor_c("fog_density"), g_cfg.misc.fog_density);
			save_int(misc, xor_c("skybox"), g_cfg.misc.skybox);
			save_string(misc, xor_c("skybox_name"), g_cfg.misc.skybox_name);
			save_string(misc, xor_c("sound_name"), g_cfg.misc.sound_name);
			save_int(misc, xor_c("prop_alpha"), g_cfg.misc.prop_alpha);
			save_uint(misc, xor_c("world_material_options"), g_cfg.misc.world_material_options);

			save_float(misc[xor_c("keybind_pos")], xor_c("x"), g_cfg.misc.keybind_position.x);
			save_float(misc[xor_c("keybind_pos")], xor_c("y"), g_cfg.misc.keybind_position.y);

			save_float(misc[xor_c("spectator_pos")], xor_c("x"), g_cfg.misc.spectators_position.x);
			save_float(misc[xor_c("spectator_pos")], xor_c("y"), g_cfg.misc.spectators_position.y);

			save_float(misc[xor_c("bomb_pos")], xor_c("x"), g_cfg.misc.bomb_position.x);
			save_float(misc[xor_c("bomb_pos")], xor_c("y"), g_cfg.misc.bomb_position.y);

			save_int(misc[xor_c("sunset_angle")], xor_c("x"), g_cfg.misc.sunset_angle.x);
			save_int(misc[xor_c("sunset_angle")], xor_c("y"), g_cfg.misc.sunset_angle.y);
			save_int(misc[xor_c("sunset_angle")], xor_c("z"), g_cfg.misc.sunset_angle.y);

			save_clr(misc, xor_c("autopeek_clr"), &g_cfg.misc.autopeek_clr);
			save_clr(misc, xor_c("autopeek_clr_back"), &g_cfg.misc.autopeek_clr_back);
			save_clr(misc, xor_c("texture_reflect_clr"), &g_cfg.misc.texture_reflect_clr);
		}

		auto& skins = json_obj[xor_c("skins")];
		{
			save_int(skins, xor_c("model_glove"), g_cfg.skins.model_glove);
			save_int(skins, xor_c("glove_skin"), g_cfg.skins.glove_skin);
			save_int(skins, xor_c("masks"), g_cfg.skins.masks);

			save_string(skins, xor_c("custom_model_t"), g_cfg.skins.custom_model_t);
			save_string(skins, xor_c("custom_model_ct"), g_cfg.skins.custom_model_ct);

			save_int(skins[xor_c("agents")], xor_c("ct"), g_cfg.skins.model_ct);
			save_int(skins[xor_c("agents")], xor_c("t"), g_cfg.skins.model_t);

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("skin_weapon ") + std::to_string(i + 1);

				auto& skins_cfg = g_cfg.skins.skin_weapon[i];
				auto& skins_cfg_json = skins[sub];
				{
					save_bool(skins_cfg_json, xor_c("enable"), skins_cfg.enable);
					save_int(skins_cfg_json, xor_c("knife_model"), skins_cfg.knife_model);
					save_int(skins_cfg_json, xor_c("skin"), skins_cfg.skin);
				}
			}
		}

		auto& binds = json_obj[xor_c("binds")];
		{
			for (int i = 0; i < binds_max; i++)
			{
				auto add = std::to_string(i + 1);
				save_bind(binds, add, g_cfg.binds[i]);
			}
		}

		std::string file = folder + config_name;
		auto str = json_obj.toStyledString();
		for (int i = 0; i < str.size(); ++i)
			str[i] ^= CFG_XOR_KEY;

		std::ofstream file_out(file);
		if (file_out.good())
			file_out << str;
		file_out.close();
	}

	void load(const std::string& config_name)
	{
		std::string file = folder + config_name;

		auto str = file_to_string(file);
		if (str.empty())
			return;

		for (int i = 0; i < str.size(); ++i)
			str[i] ^= CFG_XOR_KEY;

		std::stringstream stream{};
		stream << str;

		json json_obj{};
		stream >> json_obj;

		if (!json_obj.isMember(xor_c("rage")))
			return;

		auto& rage = json_obj[xor_c("rage")];
		{
			load_bool(rage, xor_c("enable"), g_cfg.rage.enable);
			load_bool(rage, xor_c("resolver"), g_cfg.rage.resolver);
			load_bool(rage, xor_c("auto_fire"), g_cfg.rage.auto_fire);
			load_bool(rage, xor_c("air_defensive"), g_cfg.rage.air_defensive);
			load_int(rage, xor_c("spike_amt"), g_cfg.rage.spike_amt);
			load_int(rage, xor_c("roll_amt"), g_cfg.rage.roll_amt);
			load_int(rage, xor_c("roll_amt_pitch"), g_cfg.rage.roll_amt_pitch);

#if ALPHA || _DEBUG || BETA
			load_bool(rage, xor_c("round_lagcomp"), g_cfg.rage.round_lagcomp);
			load_bool(rage, xor_c("delay_on_breaklc"), g_cfg.rage.delay_on_breaklc);
			load_bool(rage, xor_c("jitterfix"), g_cfg.rage.jitterfix);
			load_int(rage, xor_c("jitterfix_method"), g_cfg.rage.jitterfix_method);
#endif

			for (int i = 0; i < weapon_max; i++)
			{
				auto sub = xor_c("weapon_config_id ") + std::to_string(i + 1);
				load_bool(rage[sub], xor_c("enable"), g_cfg.rage.weapon[i].enable);
				load_bool(rage[sub], xor_c("quick_stop"), g_cfg.rage.weapon[i].quick_stop);
				load_bool(rage[sub], xor_c("auto_scope"), g_cfg.rage.weapon[i].auto_scope);
				load_bool(rage[sub], xor_c("prefer_body"), g_cfg.rage.weapon[i].prefer_body);
				load_bool(rage[sub], xor_c("prefer_safe"), g_cfg.rage.weapon[i].prefer_safe);
				load_bool(rage[sub], xor_c("strict_mode"), g_cfg.rage.weapon[i].strict_mode);

				load_int(rage[sub], xor_c("hitchance"), g_cfg.rage.weapon[i].hitchance);
				load_int(rage[sub], xor_c("mindamage"), g_cfg.rage.weapon[i].mindamage);
				load_int(rage[sub], xor_c("damage_override"), g_cfg.rage.weapon[i].damage_override);

				load_int(rage[sub], xor_c("scale_head"), g_cfg.rage.weapon[i].scale_head);
				load_int(rage[sub], xor_c("scale_body"), g_cfg.rage.weapon[i].scale_body);

				load_uint(rage[sub], xor_c("quick_stop_options"), g_cfg.rage.weapon[i].quick_stop_options);
				load_uint(rage[sub], xor_c("hitboxes"), g_cfg.rage.weapon[i].hitboxes);
			//	load_uint(rage[sub], xor_c("hitchance_skips"), g_cfg.rage.weapon[i].hitchance_skips);
			}
		}

		auto& legit = json_obj[xor_c("legit")];
		{
			load_bool(legit, xor_c("enable"), g_cfg.legit.enable);
			load_bool(legit, xor_c("autowall"), g_cfg.legit.autowall);
			load_bool(legit, xor_c("flash_check"), g_cfg.legit.flash_check);
			load_bool(legit, xor_c("smoke_check"), g_cfg.legit.smoke_check);

			load_int(legit, xor_c("min_damage"), g_cfg.legit.min_damage);

			auto& rcs_legit = legit[xor_c("rcs")];
			{
				load_bool(rcs_legit, xor_c("enable"), g_cfg.legit.rcs.enable);
				load_int(rcs_legit, xor_c("amount"), g_cfg.legit.rcs.amount);
			}

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("legit_weapon ") + std::to_string(i + 1);

				auto& legit_cfg = g_cfg.legit.legit_weapon[i];
				auto& legit_cfg_json = legit[sub];
				{
					load_bool(legit_cfg_json, xor_c("enable"), legit_cfg.enable);
					load_bool(legit_cfg_json, xor_c("backtrack"), legit_cfg.backtrack);
					load_bool(legit_cfg_json, xor_c("quick_stop"), legit_cfg.quick_stop);
					load_bool(legit_cfg_json, xor_c("fov_cicle"), legit_cfg.fov_cicle);

					load_int(legit_cfg_json, xor_c("fov"), legit_cfg.fov);
					load_int(legit_cfg_json, xor_c("smooth"), legit_cfg.smooth);
					load_int(legit_cfg_json, xor_c("aim_delay"), legit_cfg.aim_delay);

					load_uint(legit_cfg_json, xor_c("hitboxes"), legit_cfg.hitboxes);
					load_clr(legit_cfg_json, xor_c("circle_color"), legit_cfg.circle_color);
				}
			}
		}

		auto& antihit = json_obj[xor_c("antihit")];
		{
			load_bool(antihit, xor_c("enable"), g_cfg.antihit.enable);
			load_bool(antihit, xor_c("at_targets"), g_cfg.antihit.at_targets);
			load_bool(antihit, xor_c("silent_onshot"), g_cfg.antihit.silent_onshot);
			load_bool(antihit, xor_c("def_pitch"), g_cfg.antihit.def_pitch);
			load_bool(antihit, xor_c("def_yaw"), g_cfg.antihit.def_yaw);
			load_bool(antihit, xor_c("desync"), g_cfg.antihit.desync);
			load_bool(antihit, xor_c("distortion"), g_cfg.antihit.distortion);
			load_bool(antihit, xor_c("fakelag"), g_cfg.antihit.fakelag);
			load_bool(antihit, xor_c("jitter_move"), g_cfg.antihit.jitter_move);
			load_bool(antihit, xor_c("random_dsy"), g_cfg.antihit.random_dsy);
			load_bool(antihit, xor_c("random_amount"), g_cfg.antihit.random_amount);
			load_bool(antihit, xor_c("random_jitter"), g_cfg.antihit.random_jitter);

			load_int(antihit, xor_c("def_aa_mode"), g_cfg.antihit.def_aa_mode);
			load_int(antihit, xor_c("pitch"), g_cfg.antihit.pitch);
			load_int(antihit, xor_c("yaw"), g_cfg.antihit.yaw);
			load_int(antihit, xor_c("yaw_add"), g_cfg.antihit.yaw_add);
			load_int(antihit, xor_c("desync_mode"), g_cfg.antihit.desync_mode);
			load_int(antihit, xor_c("jitter_mode"), g_cfg.antihit.jitter_mode);
			load_int(antihit, xor_c("jitter_range"), g_cfg.antihit.jitter_range);
			load_int(antihit, xor_c("desync_left"), g_cfg.antihit.desync_left);
			load_int(antihit, xor_c("desync_right"), g_cfg.antihit.desync_right);
			load_int(antihit, xor_c("distortion_range"), g_cfg.antihit.distortion_range);
			load_int(antihit, xor_c("distortion_pitch"), g_cfg.antihit.distortion_pitch);
			load_int(antihit, xor_c("fakelag_limit"), g_cfg.antihit.fakelag_limit);
			load_uint(antihit, xor_c("fakelag_conditions"), g_cfg.antihit.fakelag_conditions);
		}

		auto& visuals = json_obj[xor_c("visuals")];
		{
			for (int i = 0; i < esp_max; ++i)
			{
				auto sub = xor_c("esp ") + std::to_string(i + 1);

				auto& esp_cfg = g_cfg.visuals.esp[i];
				auto& esp_cfg_json = visuals[sub];
				{
					load_bool(esp_cfg_json, xor_c("enable"), esp_cfg.enable);
					load_bool(esp_cfg_json, xor_c("nade_offscreen"), esp_cfg.nade_offscreen);
					load_int(esp_cfg_json, xor_c("offscreen_dist"), esp_cfg.offscreen_dist);
					load_int(esp_cfg_json, xor_c("offscreen_size"), esp_cfg.offscreen_size);
					load_uint(esp_cfg_json, xor_c("elements"), esp_cfg.elements);

					auto& esp_colors = esp_cfg_json[xor_c("colors")];
					{
						load_clr(esp_colors, xor_c("box"), esp_cfg.colors.box);
						load_clr(esp_colors, xor_c("name"), esp_cfg.colors.name);
						load_clr(esp_colors, xor_c("health"), esp_cfg.colors.health);
						load_clr(esp_colors, xor_c("weapon"), esp_cfg.colors.weapon);
						load_clr(esp_colors, xor_c("ammo_bar"), esp_cfg.colors.ammo_bar);
						load_clr(esp_colors, xor_c("offscreen_arrow"), esp_cfg.colors.offscreen_arrow);
						load_clr(esp_colors, xor_c("offscreen_arrow_outline"), esp_cfg.colors.offscreen_arrow_outline);
						load_clr(esp_colors, xor_c("glow"), esp_cfg.colors.glow);
						load_clr(esp_colors, xor_c("skeleton"), esp_cfg.colors.skeleton);
						load_clr(esp_colors, xor_c("molotov_range"), esp_cfg.colors.molotov_range);
						load_clr(esp_colors, xor_c("smoke_range"), esp_cfg.colors.smoke_range);
					}
				}
			}

			load_bool(visuals, xor_c("grenade_predict"), g_cfg.visuals.grenade_predict);
			load_clr(visuals, xor_c("predict_clr"), g_cfg.visuals.predict_clr);
			load_bool(visuals, xor_c("show_all_history"), g_cfg.visuals.show_all_history);

			load_bool(visuals, xor_c("local_glow"), g_cfg.visuals.local_glow);
			load_clr(visuals, xor_c("local_glow_color"), g_cfg.visuals.local_glow_color);

			load_bool(visuals, xor_c("grenade_warning"), g_cfg.visuals.grenade_warning);
			load_bool(visuals, xor_c("grenade_warning_line"), g_cfg.visuals.grenade_warning_line);
			load_clr(visuals, xor_c("warning_clr"), g_cfg.visuals.warning_clr);

			load_bool(visuals[xor_c("eventlog")], xor_c("enable"), g_cfg.visuals.eventlog.enable);
			load_bool(visuals[xor_c("eventlog")], xor_c("filter_console"), g_cfg.visuals.eventlog.filter_console);
			load_uint(visuals[xor_c("eventlog")], xor_c("logs"), g_cfg.visuals.eventlog.logs);

			for (int i = 0; i < c_max; ++i)
			{
				auto sub = xor_c("chams ") + std::to_string(i + 1);
				auto& chams_sub = visuals[sub];
				{
					load_bool(chams_sub, xor_c("enable"), g_cfg.visuals.chams[i].enable);
					load_int(chams_sub, xor_c("material"), g_cfg.visuals.chams[i].material);
					load_int(chams_sub, xor_c("glow_fill"), g_cfg.visuals.chams[i].glow_fill);
					load_int(chams_sub, xor_c("shot_duration"), g_cfg.visuals.chams[i].shot_duration);
					load_clr(chams_sub, xor_c("main_color"), g_cfg.visuals.chams[i].main_color);
					load_clr(chams_sub, xor_c("glow_color"), g_cfg.visuals.chams[i].glow_color);
					load_string(chams_sub, xor_c("chams_sprite"), g_cfg.visuals.chams[i].chams_sprite);
				}
			}
		}

		auto& misc = json_obj[xor_c("misc")];
		{
			load_bool(misc, xor_c("auto_jump"), g_cfg.misc.auto_jump);
			load_bool(misc, xor_c("auto_strafe"), g_cfg.misc.auto_strafe);
			load_bool(misc, xor_c("fast_stop"), g_cfg.misc.fast_stop);
			load_bool(misc, xor_c("slide_walk"), g_cfg.misc.slide_walk);
			load_bool(misc, xor_c("pen_xhair"), g_cfg.misc.pen_xhair);

			for (int i = 0; i < 3; i++)
			{
				load_int(misc[xor_c("fov") + std::to_string(i + 1)], xor_c("amt_"), g_cfg.misc.fovs[i]);
				load_int(misc[xor_c("viewmodel_pos") + std::to_string(i + 1)], xor_c("pos_"), g_cfg.misc.viewmodel_pos[i]);
			}

			load_int(misc, xor_c("aspect_ratio"), g_cfg.misc.aspect_ratio);

			load_bool(misc, xor_c("thirdperson_dead"), g_cfg.misc.thirdperson_dead);
			load_int(misc, xor_c("thirdperson_dist"), g_cfg.misc.thirdperson_dist);

			load_bool(misc, xor_c("impacts"), g_cfg.misc.impacts);
			load_int(misc, xor_c("impact_size"), g_cfg.misc.impact_size);
			load_clr(misc, xor_c("server_clr"), g_cfg.misc.server_clr);
			load_clr(misc, xor_c("client_clr"), g_cfg.misc.client_clr);

			load_uint(misc, xor_c("tracers"), g_cfg.misc.tracers);
			load_int(misc, xor_c("tracer_type"), g_cfg.misc.tracer_type);

			for (int i = 0; i < 3; i++)
				load_clr(misc[xor_c("trace_clr") + std::to_string(i + 1)], xor_c("clr_"), g_cfg.misc.trace_clr[i]);

			for (int i = 0; i < world_clr_max; i++)
				load_clr(misc[xor_c("world_clr") + std::to_string(i + 1)], xor_c("clr_"), g_cfg.misc.world_clr[i]);

			load_uint(misc, xor_c("hitmarker"), g_cfg.misc.hitmarker);
			load_clr(misc, xor_c("hitmarker_clr"), g_cfg.misc.hitmarker_clr);
			load_bool(misc, xor_c("damage"), g_cfg.misc.damage);
			load_clr(misc, xor_c("damage_clr"), g_cfg.misc.damage_clr);
			load_int(misc, xor_c("sound"), g_cfg.misc.sound);
			load_int(misc, xor_c("sound_volume"), g_cfg.misc.sound_volume);
			load_bool(misc, xor_c("unlock_inventory"), g_cfg.misc.unlock_inventory);
			load_bool(misc, xor_c("clantag"), g_cfg.misc.clantag);
			load_bool(misc, xor_c("spoof_sv_cheats"), g_cfg.misc.spoof_sv_cheats);
			load_bool(misc, xor_c("unlock_hidden_cvars"), g_cfg.misc.unlock_hidden_cvars);
			load_bool(misc, xor_c("bypass_sv_pure"), g_cfg.misc.bypass_sv_pure);
			load_bool(misc, xor_c("remove_ads"), g_cfg.misc.remove_ads);
			load_bool(misc, xor_c("snip_crosshair"), g_cfg.misc.snip_crosshair);
			load_bool(misc, xor_c("preverse_killfeed"), g_cfg.misc.preverse_killfeed);
			load_bool(misc, xor_c("force_radar"), g_cfg.misc.force_radar);
			load_int(misc, xor_c("ragdoll_gravity"), g_cfg.misc.ragdoll_gravity);
			load_int(misc, xor_c("strafe_smooth"), g_cfg.misc.strafe_smooth);

			load_bool(misc[xor_c("buybot")], xor_c("enable"), g_cfg.misc.buybot.enable);
			load_int(misc[xor_c("buybot")], xor_c("main_weapon"), g_cfg.misc.buybot.main_weapon);
			load_int(misc[xor_c("buybot")], xor_c("second_weapon"), g_cfg.misc.buybot.second_weapon);
			load_uint(misc[xor_c("buybot")], xor_c("other_items"), g_cfg.misc.buybot.other_items);

			load_uint(misc, xor_c("removals"), g_cfg.misc.removals);
			load_uint(misc, xor_c("world_modulation"), g_cfg.misc.world_modulation);

			load_bool(misc, xor_c("blend_scope"), g_cfg.misc.blend_scope);
			load_int(misc, xor_c("scope_amt"), g_cfg.misc.scope_amt);
			load_int(misc, xor_c("attachments_amt"), g_cfg.misc.attachments_amt);

			load_int(misc, xor_c("bloom_scale"), g_cfg.misc.bloom_scale);
			load_bool(misc, xor_c("custom_bloom"), g_cfg.misc.custom_bloom);
			load_bool(misc, xor_c("retrack_peek"), g_cfg.misc.retrack_peek);
			load_bool(misc, xor_c("viewmodel_scope"), g_cfg.misc.viewmodel_scope);
			load_bool(misc, xor_c("fix_sensitivity"), g_cfg.misc.fix_sensitivity);
			load_bool(misc, xor_c("skip_second_zoom"), g_cfg.misc.skip_second_zoom);
			load_int(misc, xor_c("exposure_min"), g_cfg.misc.exposure_min);
			load_int(misc, xor_c("exposure_max"), g_cfg.misc.exposure_max);
			load_clr(misc, xor_c("accent_color"), g_cfg.misc.ui_color);
			load_uint(misc, xor_c("menu_indicators"), g_cfg.misc.menu_indicators);
			load_uint(misc, xor_c("animation_changes"), g_cfg.misc.animation_changes);

			load_bool(misc, xor_c("custom_fog"), g_cfg.misc.custom_fog);
			load_int(misc, xor_c("fog_start"), g_cfg.misc.fog_start);
			load_int(misc, xor_c("fog_end"), g_cfg.misc.fog_end);
			load_int(misc, xor_c("fog_density"), g_cfg.misc.fog_density);
			load_int(misc, xor_c("skybox"), g_cfg.misc.skybox);
			load_string(misc, xor_c("skybox_name"), g_cfg.misc.skybox_name);
			load_string(misc, xor_c("sound_name"), g_cfg.misc.sound_name);
			load_int(misc, xor_c("prop_alpha"), g_cfg.misc.prop_alpha);
			load_uint(misc, xor_c("world_material_options"), g_cfg.misc.world_material_options);

			load_float(misc[xor_c("keybind_pos")], xor_c("x"), g_cfg.misc.keybind_position.x);
			load_float(misc[xor_c("keybind_pos")], xor_c("y"), g_cfg.misc.keybind_position.y);

			load_float(misc[xor_c("spectator_pos")], xor_c("x"), g_cfg.misc.spectators_position.x);
			load_float(misc[xor_c("spectator_pos")], xor_c("y"), g_cfg.misc.spectators_position.y);

			load_int(misc[xor_c("sunset_angle")], xor_c("x"), g_cfg.misc.sunset_angle.x);
			load_int(misc[xor_c("sunset_angle")], xor_c("y"), g_cfg.misc.sunset_angle.y);
			load_int(misc[xor_c("sunset_angle")], xor_c("z"), g_cfg.misc.sunset_angle.y);

			load_float(misc[xor_c("bomb_pos")], xor_c("x"), g_cfg.misc.bomb_position.x);
			load_float(misc[xor_c("bomb_pos")], xor_c("y"), g_cfg.misc.bomb_position.y);

			load_clr(misc, xor_c("autopeek_clr"), g_cfg.misc.autopeek_clr);
			load_clr(misc, xor_c("autopeek_clr_back"), g_cfg.misc.autopeek_clr_back);
			load_clr(misc, xor_c("texture_reflect_clr"), g_cfg.misc.texture_reflect_clr);
		}

		auto& skins = json_obj[xor_c("skins")];
		{
			load_int(skins, xor_c("model_glove"), g_cfg.skins.model_glove);
			load_int(skins, xor_c("glove_skin"), g_cfg.skins.glove_skin);
			load_int(skins, xor_c("masks"), g_cfg.skins.masks);

			load_string(skins, xor_c("custom_model_t"), g_cfg.skins.custom_model_t);
			load_string(skins, xor_c("custom_model_ct"), g_cfg.skins.custom_model_ct);

			load_int(skins[xor_c("agents")], xor_c("ct"), g_cfg.skins.model_ct);
			load_int(skins[xor_c("agents")], xor_c("t"), g_cfg.skins.model_t);

			for (int i = 0; i < weapon_cfg_max; ++i)
			{
				auto sub = xor_c("skin_weapon ") + std::to_string(i + 1);

				auto& skins_cfg = g_cfg.skins.skin_weapon[i];
				auto& skins_cfg_json = skins[sub];
				{
					load_bool(skins_cfg_json, xor_c("enable"), skins_cfg.enable);
					load_int(skins_cfg_json, xor_c("knife_model"), skins_cfg.knife_model);
					load_int(skins_cfg_json, xor_c("skin"), skins_cfg.skin);
				}
			}
		}

		auto& binds = json_obj[xor_c("binds")];
		{
			for (int i = 0; i < binds_max; i++)
			{
				auto add = std::to_string(i + 1);
				load_bind(binds, add, g_cfg.binds[i]);
			}
		}
	}
}