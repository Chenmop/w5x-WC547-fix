/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"

enum HunterPetCalculate
{
     SPELL_TAMED_PET_PASSIVE_06         = 19591,
     SPELL_TAMED_PET_PASSIVE_07         = 20784,
     SPELL_TAMED_PET_PASSIVE_08         = 34666,
     SPELL_TAMED_PET_PASSIVE_09         = 34667,
     SPELL_TAMED_PET_PASSIVE_10         = 34675,
     SPELL_HUNTER_PET_SCALING_01        = 34902,
     SPELL_HUNTER_PET_SCALING_02        = 34903,
     SPELL_HUNTER_PET_SCALING_03        = 34904,
     SPELL_HUNTER_PET_SCALING_04        = 61017,
     SPELL_HUNTER_ANIMAL_HANDLER        = 34453,
};

enum WarlockPetCalculate
{
     SPELL_PET_PASSIVE_CRIT             = 35695,
     SPELL_PET_PASSIVE_DAMAGE_TAKEN     = 35697,
     SPELL_WARLOCK_PET_SCALING_01       = 34947,
     SPELL_WARLOCK_PET_SCALING_02       = 34956,
     SPELL_WARLOCK_PET_SCALING_03       = 34957,
     SPELL_WARLOCK_PET_SCALING_04       = 34958,
     SPELL_WARLOCK_PET_SCALING_05       = 61013,
     SPELL_WARLOCK_GLYPH_OF_VOIDWALKER  = 56247,
};

enum DKPetCalculate
{
    SPELL_DEATH_KNIGHT_RUNE_WEAPON_02   = 51906,
    SPELL_DEATH_KNIGHT_PET_SCALING_01   = 54566,
    SPELL_DEATH_KNIGHT_PET_SCALING_02   = 51996,
    SPELL_DEATH_KNIGHT_PET_SCALING_03   = 61697,
    SPELL_NIGHT_OF_THE_DEAD             = 55620,
    ENTRY_ARMY_OF_THE_DEAD_GHOUL        = 24207,
    SPELL_DEATH_KNIGHT_GLYPH_OF_GHOUL   = 58686,
};

enum ShamanPetCalculate
{
    SPELL_FERAL_SPIRIT_PET_UNK_01      = 35674,
    SPELL_FERAL_SPIRIT_PET_UNK_02      = 35675,
    SPELL_FERAL_SPIRIT_PET_UNK_03      = 35676,
    SPELL_FERAL_SPIRIT_PET_SCALING_04  = 61783,
};

enum MiscPetCalculate
{
     SPELL_MAGE_PET_PASSIVE_ELEMENTAL   = 44559,
     SPELL_PET_HEALTH_SCALING           = 61679,
     SPELL_PET_UNK_01                   = 67561,
     SPELL_PET_UNK_02                   = 67557,
};

class spell_gen_pet_calculate : public SpellScriptLoader
{
    public:
        spell_gen_pet_calculate() : SpellScriptLoader("spell_gen_pet_calculate") { }

        class spell_gen_pet_calculate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_pet_calculate_AuraScript);

            bool Load()
            {
                if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                    return false;
                return true;
            }

            bool CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float CritSpell = 0.0f;
                    // Crit from Intellect
                    CritSpell += owner->GetSpellCritFromIntellect();
                    // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
                    CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                    // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                    CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                    // Increase crit spell from spell crit ratings
                    CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

                    amount += int32(CritSpell);
                    return true;
                }
                return false;
            }

            bool CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float CritMelee = 0.0f;
                    // Crit from Agility
                    CritMelee += owner->GetMeleeCritFromAgility();
                    // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
                    CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                    // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                    CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                    // Increase crit melee from melee crit ratings
                    CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

                    amount += int32(CritMelee);
                    return true;
                }
                return false;
            }

            bool CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float HitMelee = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                    HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                    // Increase hit melee from meele hit ratings
                    HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                    amount += int32(HitMelee);
                    return true;
                }
                return false;
            }

            bool CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float HitSpell = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                    HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                    // Increase hit spell from spell hit ratings
                    HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                    amount += int32(HitSpell);
                    return true;
                }
                return false;
            }

            bool CalculateAmountExpertise(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float Expertise = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_EXPERTISE
                    Expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
                    // Increase Expertise from Expertise ratings
                    Expertise += owner->GetRatingBonusValue(CR_EXPERTISE);

                    amount += int32(Expertise);
                    return true;
                }

                return false;
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case SPELL_TAMED_PET_PASSIVE_06:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritMelee, EFFECT_0, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritSpell, EFFECT_1, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                        break;
                    case SPELL_PET_PASSIVE_CRIT:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritSpell, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritMelee, EFFECT_1, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                        break;
                    case SPELL_WARLOCK_PET_SCALING_05:
                    case SPELL_HUNTER_PET_SCALING_04:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountExpertise, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
                        break;
                    case SPELL_DEATH_KNIGHT_PET_SCALING_03:
//                    case SPELL_SHAMAN_PET_HIT:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_pet_calculate_AuraScript();
        }
};

class spell_warl_pet_scaling_01 : public SpellScriptLoader
{
public:
    spell_warl_pet_scaling_01() : SpellScriptLoader("spell_warl_pet_scaling_01") { }

    class spell_warl_pet_scaling_01_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_01_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            _tempBonus = 0;
            return true;
        }

        bool CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = CalculatePct(owner->GetStat(STAT_STAMINA), 75);

                        amount += ownerBonus;
                    }
        }

        void ApplyEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (_tempBonus)
                {
                    PetLevelInfo const* pInfo = sObjectMgr->GetPetLevelInfo(pet->GetEntry(), pet->getLevel());
                    uint32 healthMod = 0;
                    uint32 baseHealth = pInfo->health;
                    switch (pet->GetEntry())
                    {
                        case ENTRY_IMP:
                            healthMod = uint32(_tempBonus * 8.4f);
                            break;
                        case ENTRY_FELGUARD:
                        case ENTRY_VOIDWALKER:
                            healthMod = _tempBonus * 11;
                            break;
                        case ENTRY_SUCCUBUS:
                            healthMod = uint32(_tempBonus * 9.1f);
                            break;
                        case ENTRY_FELHUNTER:
                            healthMod = uint32(_tempBonus * 9.5f);
                            break;
                        default:
                            healthMod = 0;
                            break;
                    }
                    if (healthMod)
                        pet->ToPet()->SetCreateHealth(baseHealth + healthMod);
                }
        }

        void RemoveEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                {
                    PetLevelInfo const* pInfo = sObjectMgr->GetPetLevelInfo(pet->GetEntry(), pet->getLevel());
                    pet->ToPet()->SetCreateHealth(pInfo->health);
                }
        }

        bool CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())

                if (Unit* owner = pet->ToPet()->GetOwner())
                {
                    int32 fire  = int32(owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SPELL_SCHOOL_FIRE)) - owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + SPELL_SCHOOL_FIRE);
                    int32 shadow = int32(owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SPELL_SCHOOL_SHADOW)) - owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + SPELL_SCHOOL_SHADOW);
                    int32 maximum  = (fire > shadow) ? fire : shadow;
                    if (maximum < 0)
                        maximum = 0;
                    float bonusAP = maximum * 0.57f;

                    amount += bonusAP;

                    // Glyph of felguard
                    if (pet->GetEntry() == ENTRY_FELGUARD)
                    {
                        if (AuraEffect* /* aurEff */ect = owner->GetAuraEffect(56246, EFFECT_0))
                        {
                            float base_attPower = pet->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE) * pet->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_PCT);
                            amount += CalculatePct(amount+base_attPower, /* aurEff */ect->GetAmount());
                        }
                    }
                }
        }

        bool CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        //the damage bonus used for pets is either fire or shadow damage, whatever is higher
                        int32 fire  = int32(owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SPELL_SCHOOL_FIRE)) - owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + SPELL_SCHOOL_FIRE);
                        int32 shadow = int32(owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SPELL_SCHOOL_SHADOW)) - owner->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + SPELL_SCHOOL_SHADOW);
                        int32 maximum  = (fire > shadow) ? fire : shadow;
                        float bonusDamage = 0.0f;

                        if (maximum > 0)
                            bonusDamage = maximum * 0.15f;

                        amount += bonusDamage;
                    }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_pet_scaling_01_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectApply += AuraEffectApplyFn(spell_warl_pet_scaling_01_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
        }

    private:
        uint32 _tempBonus;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_scaling_01_AuraScript();
    }
};

class spell_warl_pet_scaling_02 : public SpellScriptLoader
{
public:
    spell_warl_pet_scaling_02() : SpellScriptLoader("spell_warl_pet_scaling_02") { }

    class spell_warl_pet_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_02_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            _tempBonus = 0;
            return true;
        }

        bool CalculateIntellectAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;

                        ownerBonus = CalculatePct(owner->GetStat(STAT_INTELLECT), 30);

                        amount += ownerBonus;
                        _tempBonus = ownerBonus;
                    }
        }

        void ApplyEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (_tempBonus)
                {
                    PetLevelInfo const* pInfo = sObjectMgr->GetPetLevelInfo(pet->GetEntry(), pet->getLevel());
                    uint32 manaMod = 0;
                    uint32 baseMana = pInfo->mana;
                    switch (pet->GetEntry())
                    {
                    case ENTRY_IMP:
                        manaMod = uint32(_tempBonus * 4.9f);
                        break;
                    case ENTRY_VOIDWALKER:
                    case ENTRY_SUCCUBUS:
                    case ENTRY_FELHUNTER:
                    case ENTRY_FELGUARD:
                        manaMod = uint32(_tempBonus * 11.5f);
                        break;
                    default:
                        manaMod = 0;
                        break;
                    }
                    if (manaMod)
                        pet->ToPet()->SetCreateMana(baseMana + manaMod);
                }
        }

        void RemoveEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                {
                    PetLevelInfo const* pInfo = sObjectMgr->GetPetLevelInfo(pet->GetEntry(), pet->getLevel());
                    pet->ToPet()->SetCreateMana(pInfo->mana);
                }
        }

        bool CalculateArmorAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetArmor(), 35);
                        amount += ownerBonus;
                    }
        }

        bool CalculateFireResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_FIRE), 40);
                        amount += ownerBonus;
                    }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_pet_scaling_02_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectApply += AuraEffectApplyFn(spell_warl_pet_scaling_02_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateIntellectAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateArmorAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateFireResistanceAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
        }

    private:
        uint32 _tempBonus;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_scaling_02_AuraScript();
    }
};

class spell_warl_pet_scaling_03 : public SpellScriptLoader
{
public:
    spell_warl_pet_scaling_03() : SpellScriptLoader("spell_warl_pet_scaling_03") { }

    class spell_warl_pet_scaling_03_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_03_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateFrostResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_FROST), 40);
                        amount += ownerBonus;
                    }
        }

        bool CalculateArcaneResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_ARCANE), 40);
                        amount += ownerBonus;
                    }
        }

        bool CalculateNatureResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_NATURE), 40);
                        amount += ownerBonus;
                    }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_03_AuraScript::CalculateFrostResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_03_AuraScript::CalculateArcaneResistanceAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_03_AuraScript::CalculateNatureResistanceAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_scaling_03_AuraScript();
    }
};


class spell_warl_pet_scaling_04 : public SpellScriptLoader
{
public:
    spell_warl_pet_scaling_04() : SpellScriptLoader("spell_warl_pet_scaling_04") { }

    class spell_warl_pet_scaling_04_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_04_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateShadowResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float ownerBonus = 0.0f;
                        ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_SHADOW), 40);
                        amount += ownerBonus;
                    }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_04_AuraScript::CalculateShadowResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_scaling_04_AuraScript();
    }
};

class spell_warl_pet_scaling_05 : public SpellScriptLoader
{
public:
    spell_warl_pet_scaling_05() : SpellScriptLoader("spell_warl_pet_scaling_05") { }

    class spell_warl_pet_scaling_05_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_05_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitMelee += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitMelee);
            }
        }

        bool CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitSpell = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitSpell);
            }
        }

        bool CalculateAmountExpertise(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float Expertise = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                Expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                Expertise += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(Expertise);
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_05_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_05_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_05_AuraScript::CalculateAmountExpertise, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_scaling_05_AuraScript();
    }
};

class spell_warl_pet_passive : public SpellScriptLoader
{
public:
    spell_warl_pet_passive() : SpellScriptLoader("spell_warl_pet_passive") { }

    class spell_warl_pet_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_passive_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float CritSpell = 0.0f;
                // Crit from Intellect
                CritSpell += owner->GetSpellCritFromIntellect();
                // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
                CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                // Increase crit spell from spell crit ratings
                CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

                if (AuraApplication* improvedDemonicTacticsApp = owner->GetAuraApplicationOfRankedSpell(54347))
                    if (Aura* improvedDemonicTactics = improvedDemonicTacticsApp->GetBase())
                        if (AuraEffect* improvedDemonicTacticsEffect = improvedDemonicTactics->GetEffect(EFFECT_0))
                            amount += CalculatePct(CritSpell, improvedDemonicTacticsEffect->GetAmount());
            }
        }

        bool CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float CritMelee = 0.0f;
                // Crit from Agility
                CritMelee += owner->GetMeleeCritFromAgility();
                // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
                CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                // Increase crit melee from melee crit ratings
                CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

                if (AuraApplication* improvedDemonicTacticsApp = owner->GetAuraApplicationOfRankedSpell(54347))
                    if (Aura* improvedDemonicTactics = improvedDemonicTacticsApp->GetBase())
                        if (AuraEffect* improvedDemonicTacticsEffect = improvedDemonicTactics->GetEffect(EFFECT_0))
                            amount += CalculatePct(CritMelee, improvedDemonicTacticsEffect->GetAmount());
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_passive_AuraScript::CalculateAmountCritSpell, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_passive_AuraScript::CalculateAmountCritMelee, EFFECT_1, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_passive_AuraScript();
    }
};
// this doesnt actually fit in here
class spell_warl_pet_passive_damage_done : public SpellScriptLoader
{
public:
    spell_warl_pet_passive_damage_done() : SpellScriptLoader("spell_warl_pet_passive_damage_done") { }

    class spell_warl_pet_passive_damage_done_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_passive_damage_done_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountDamageDone(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (GetCaster()->GetOwner()->ToPlayer())
            {
                switch (GetCaster()->GetEntry())
                {
                case ENTRY_VOIDWALKER:
                    amount += -16;
                    break;
                case ENTRY_FELHUNTER:
                    amount += -20;
                    break;
                case ENTRY_SUCCUBUS:
                case ENTRY_FELGUARD:
                    amount += 5;
                    break;
                }
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_passive_damage_done_AuraScript::CalculateAmountDamageDone, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_passive_damage_done_AuraScript::CalculateAmountDamageDone, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_passive_damage_done_AuraScript();
    }
};

class spell_warl_pet_passive_voidwalker : public SpellScriptLoader
{
public:
    spell_warl_pet_passive_voidwalker() : SpellScriptLoader("spell_warl_pet_passive_voidwalker") { }

    class spell_warl_pet_passive_voidwalker_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_passive_voidwalker_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                        if (AuraEffect* /* aurEff */ect = owner->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_VOIDWALKER, EFFECT_0))
                            amount += /* aurEff */ect->GetAmount();
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_passive_voidwalker_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_pet_passive_voidwalker_AuraScript();
    }
};


class spell_sha_pet_scaling_04 : public SpellScriptLoader
{
public:
    spell_sha_pet_scaling_04() : SpellScriptLoader("spell_sha_pet_scaling_04") { }

    class spell_sha_pet_scaling_04_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_pet_scaling_04_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                // Increase hit melee from meele hit ratings
                HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                amount += int32(HitMelee);
            }
        }

        bool CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitSpell = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitSpell);
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_pet_scaling_04_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_pet_scaling_04_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_sha_pet_scaling_04_AuraScript();
    }
};

class spell_hun_pet_scaling_01 : public SpellScriptLoader
{
public:
    spell_hun_pet_scaling_01() : SpellScriptLoader("spell_hun_pet_scaling_01") { }

    class spell_hun_pet_scaling_01_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_scaling_01_AuraScript);

        bool CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (pet->isPet())
                    if (Unit* owner = pet->ToPet()->GetOwner())
                    {
                        float mod = 0.45f;
                        float ownerBonus = 0.0f;

                        PetSpellMap::const_iterator itr = (pet->ToPet()->m_spells.find(62758)); // Wild Hunt rank 1
                        if (itr == pet->ToPet()->m_spells.end())
                            itr = pet->ToPet()->m_spells.find(62762); // Wild Hunt rank 2

                        if (itr != pet->ToPet()->m_spells.end()) // If pet has Wild Hunt
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                            AddPct(mod, spellInfo->Effects[EFFECT_0].CalcValue());
                        }

                        ownerBonus = owner->GetStat(STAT_STAMINA)*mod;

                        amount += ownerBonus;
                    }
        }

        void ApplyEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (_tempHealth)
                    pet->SetHealth(_tempHealth);
        }

        void RemoveEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                _tempHealth = pet->GetHealth();
        }

        bool CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float mod = 1.0f;                                                 //Hunter contribution modifier
                float bonusAP = 0.0f;

                PetSpellMap::const_iterator itr = (pet->ToPet()->m_spells.find(62758)); // Wild Hunt rank 1
                if (itr == pet->ToPet()->m_spells.end())
                    itr = pet->ToPet()->m_spells.find(62762); // Wild Hunt rank 2

                if (itr != pet->ToPet()->m_spells.end()) // If pet has Wild Hunt
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                    mod += CalculatePct(1.0f, spellInfo->Effects[EFFECT_1].CalcValue());
                }

                bonusAP = owner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.22f * mod;

                amount += bonusAP;

                return true;
            }

            return false;
        }

        bool CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float mod = 1.0f;                                                 //Hunter contribution modifier
                float bonusDamage = 0.0f;

                PetSpellMap::const_iterator itr = (pet->ToPet()->m_spells.find(62758)); // Wild Hunt rank 1
                if (itr == pet->ToPet()->m_spells.end())
                    itr = pet->ToPet()->m_spells.find(62762); // Wild Hunt rank 2

                if (itr != pet->ToPet()->m_spells.end()) // If pet has Wild Hunt
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                    mod += CalculatePct(1.0f, spellInfo->Effects[EFFECT_1].CalcValue());
                }

                bonusDamage = owner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.1287f * mod;

                amount += bonusDamage;
                return true;
            }

            return false;
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_pet_scaling_01_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectApply += AuraEffectApplyFn(spell_hun_pet_scaling_01_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
        }

    private:
        uint32 _tempHealth;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_scaling_01_AuraScript();
    }
};

class spell_hun_pet_scaling_02 : public SpellScriptLoader
{
public:
    spell_hun_pet_scaling_02() : SpellScriptLoader("spell_hun_pet_scaling_02") { }

    class spell_hun_pet_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_scaling_02_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateFrostResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_FROST), 40);

                amount += ownerBonus;
                return true;
            }

            return false;
        }

        bool CalculateFireResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_FIRE), 40);

                amount += ownerBonus;
                return true;
            }

            return false;
        }

        bool CalculateNatureResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_NATURE), 40);

                amount += ownerBonus;

                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_02_AuraScript::CalculateFrostResistanceAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_02_AuraScript::CalculateFireResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_02_AuraScript::CalculateNatureResistanceAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_scaling_02_AuraScript();
    }
};

class spell_hun_pet_scaling_03 : public SpellScriptLoader
{
public:
    spell_hun_pet_scaling_03() : SpellScriptLoader("spell_hun_pet_scaling_03") { }

    class spell_hun_pet_scaling_03_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_scaling_03_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateShadowResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_SHADOW), 40);

                amount += ownerBonus;
                return true;
            }

            return false;
        }

        bool CalculateArcaneResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_ARCANE), 40);

                amount += ownerBonus;

                return true;
            }

            return false;
        }

        bool CalculateArmorAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isPet())
                    return false;

                Unit* owner = pet->ToPet()->GetOwner();
                if (!owner)
                    return false;

                float ownerBonus = 0.0f;

                ownerBonus = CalculatePct(owner->GetArmor(), 35);

                amount += ownerBonus;
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_03_AuraScript::CalculateShadowResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_03_AuraScript::CalculateArcaneResistanceAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_03_AuraScript::CalculateArmorAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_scaling_03_AuraScript();
    }
};

class spell_hun_pet_scaling_04 : public SpellScriptLoader
{
public:
    spell_hun_pet_scaling_04() : SpellScriptLoader("spell_hun_pet_scaling_04") { }

    class spell_hun_pet_scaling_04_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_scaling_04_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                // Increase hit melee from meele hit ratings
                HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                amount += int32(HitMelee);
                return true;
            }

            return false;
        }

        bool CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitSpell = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitSpell);

                return true;
            }

            return false;
        }

        bool CalculateAmountExpertise(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float Expertise = 0.0f;
                // Increase hit from SPELL_AURA_MOD_EXPERTISE
                Expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
                // Increase Expertise from Expertise ratings
                Expertise += owner->GetRatingBonusValue(CR_EXPERTISE);

                amount += int32(Expertise);
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_04_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_04_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_04_AuraScript::CalculateAmountExpertise, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_scaling_04_AuraScript();
    }
};

class spell_hun_pet_passive_crit : public SpellScriptLoader
{
public:
    spell_hun_pet_passive_crit() : SpellScriptLoader("spell_hun_pet_passive_crit") { }

    class spell_hun_pet_passive_crit_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_passive_crit_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float CritSpell = 0.0f;
                // Crit from Intellect
                // CritSpell += owner->GetSpellCritFromIntellect();
                // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
                // CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                // CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                // Increase crit spell from spell crit ratings
                // CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

                amount += (CritSpell*0.8f);
                return true;
            }

            return false;
        }

        bool CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float CritMelee = 0.0f;
                // Crit from Agility
                // CritMelee += owner->GetMeleeCritFromAgility();
                // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
                // CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                // CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                // Increase crit melee from melee crit ratings
                // CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

                amount += (CritMelee*0.8f);
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_passive_crit_AuraScript::CalculateAmountCritSpell, EFFECT_1, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_passive_crit_AuraScript::CalculateAmountCritMelee, EFFECT_0, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_passive_crit_AuraScript();
    }
};

class spell_hun_pet_passive_damage_done : public SpellScriptLoader
{
public:
    spell_hun_pet_passive_damage_done() : SpellScriptLoader("spell_hun_pet_passive_damage_done") { }

    class spell_hun_pet_passive_damage_done_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_passive_damage_done_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountDamageDone(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (GetCaster()->GetOwner()->ToPlayer())
            {
                // Cobra Reflexes
                if (AuraEffect* cobraReflexes = GetCaster()->GetAuraEffectOfRankedSpell(61682, EFFECT_0))
                    amount -= cobraReflexes->GetAmount();

                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_passive_damage_done_AuraScript::CalculateAmountDamageDone, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_passive_damage_done_AuraScript();
    }
};

class spell_hun_animal_handler : public SpellScriptLoader
{
public:
    spell_hun_animal_handler() : SpellScriptLoader("spell_hun_animal_handler") { }

    class spell_hun_animal_handler_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_animal_handler_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountDamageDone(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                if (AuraEffect* /* aurEff */ect = owner->GetAuraEffectOfRankedSpell(SPELL_HUNTER_ANIMAL_HANDLER, EFFECT_1))
                    amount = /* aurEff */ect->GetAmount();
                else
                    amount = 0;
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_animal_handler_AuraScript::CalculateAmountDamageDone, EFFECT_0, SPELL_AURA_MOD_ATTACK_POWER_PCT);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_animal_handler_AuraScript();
    }
};


class spell_dk_avoidance_passive : public SpellScriptLoader
{
public:
    spell_dk_avoidance_passive() : SpellScriptLoader("spell_dk_avoidance_passive") { }

    class spell_dk_avoidance_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_avoidance_passive_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAvoidanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (Unit* owner = pet->GetOwner())
                {
                    // Army of the dead ghoul
                    if (pet->GetEntry() == ENTRY_ARMY_OF_THE_DEAD_GHOUL)
                        amount = -90;
                    // Night of the dead
                    else if (Aura* aur = owner->GetAuraOfRankedSpell(SPELL_NIGHT_OF_THE_DEAD))
                        amount = aur->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                }
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_avoidance_passive_AuraScript::CalculateAvoidanceAmount, EFFECT_0, SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_avoidance_passive_AuraScript();
    }
};

class spell_dk_pet_scaling_01 : public SpellScriptLoader
{
public:
    spell_dk_pet_scaling_01() : SpellScriptLoader("spell_dk_pet_scaling_01") { }

    class spell_dk_pet_scaling_01_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_pet_scaling_01_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            _tempHealth = 0;
            return true;
        }

        bool CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (pet->isGuardian())
                {
                    if (Unit* owner = pet->GetOwner())
                    {
                        float mod = 0.3f;

                        // Ravenous Dead. Check just if owner has Ravenous Dead since it's effect is not an aura
                        if (AuraEffect const* aurEff = owner->GetAuraEffect(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, SPELLFAMILY_DEATHKNIGHT, 3010, 0))
                        {
                            mod += aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue()/100;                   // Ravenous Dead edits the original scale
                        }
                        // Glyph of the Ghoul
                        if (AuraEffect const* aurEff = owner->GetAuraEffect(SPELL_DEATH_KNIGHT_GLYPH_OF_GHOUL, 0))
                            mod += aurEff->GetAmount()/100;

                        float ownerBonus = float(owner->GetStat(STAT_STAMINA)) * mod;
                        amount += ownerBonus;
                    }
                }
            }
        }

        void ApplyEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                if (_tempHealth)
                    pet->SetHealth(_tempHealth);
        }

        void RemoveEffect(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pet = GetUnitOwner())
                _tempHealth = pet->GetHealth();
        }

        bool CalculateStrengthAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                if (!pet->isGuardian())
                    return false;

                Unit* owner = pet->GetOwner();
                if (!owner)
                    return false;

                float mod = 0.7f;

                // Ravenous Dead
                AuraEffect const* aurEff = nullptr;
                // Check just if owner has Ravenous Dead since it's effect is not an aura
                aurEff = owner->GetAuraEffect(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, SPELLFAMILY_DEATHKNIGHT, 3010, 0);
                if (aurEff)
                {
                    mod += CalculatePct(mod, aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue());                   // Ravenous Dead edits the original scale
                }
                // Glyph of the Ghoul
                aurEff = owner->GetAuraEffect(58686, 0);
                if (aurEff)
                    mod += CalculatePct(1.0f, aurEff->GetAmount());                                                    // Glyph of the Ghoul adds a flat value to the scale mod
                float ownerBonus = float(owner->GetStat(STAT_STRENGTH)) * mod;
                amount += ownerBonus;

                return true;
            }

            return false;
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_pet_scaling_01_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectApply += AuraEffectApplyFn(spell_dk_pet_scaling_01_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_01_AuraScript::CalculateStrengthAmount, EFFECT_1, SPELL_AURA_MOD_STAT);
        }

    private:
        uint32 _tempHealth;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_pet_scaling_01_AuraScript();
    }
};

class spell_dk_pet_scaling_02 : public SpellScriptLoader
{
public:
    spell_dk_pet_scaling_02() : SpellScriptLoader("spell_dk_pet_scaling_02") { }

    class spell_dk_pet_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_pet_scaling_02_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HasteMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HasteMelee += (1-owner->m_modAttackSpeedPct[BASE_ATTACK])*100;

                amount += int32(HasteMelee);
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_02_AuraScript::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_pet_scaling_02_AuraScript();
    }
};

class spell_dk_pet_scaling_03 : public SpellScriptLoader
{
public:
    spell_dk_pet_scaling_03() : SpellScriptLoader("spell_dk_pet_scaling_03") { }

    class spell_dk_pet_scaling_03_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_pet_scaling_03_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                // Increase hit melee from meele hit ratings
                HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                amount += int32(HitMelee);

                return true;
            }

            return false;
        }

        bool CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitSpell = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitSpell);
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_03_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_03_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_pet_scaling_03_AuraScript();
    }
};

class spell_dk_rune_weapon_scaling_02 : public SpellScriptLoader
{
public:
    spell_dk_rune_weapon_scaling_02() : SpellScriptLoader("spell_dk_rune_weapon_scaling_02") { }

    class spell_dk_rune_weapon_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_rune_weapon_scaling_02_AuraScript);

        bool Load()
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        bool CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetUnitOwner())
            {
                Unit* owner = pet->GetOwner();
                if (!owner)
                    return false;

                if (pet->isGuardian())
                    ((Guardian*)pet)->SetBonusDamage(owner->GetTotalAttackPowerValue(BASE_ATTACK));

                amount += owner->CalculateDamage(BASE_ATTACK, true, true);
                return true;
            }

            return false;
        }

        bool CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster() || !GetCaster()->GetOwner())
                return false;

            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HasteMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HasteMelee += (1-owner->m_modAttackSpeedPct[BASE_ATTACK])*100;

                amount += int32(HasteMelee);
                return true;
            }

            return false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_rune_weapon_scaling_02_AuraScript::CalculateDamageDoneAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_rune_weapon_scaling_02_AuraScript::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_rune_weapon_scaling_02_AuraScript();
    }
};

void AddSC_pet_spell_scripts()
{
    new spell_gen_pet_calculate();
}
