import React, {Component} from 'react'
import styled from 'styled-components'
import WifiIcon from './wifi-icon'
import posed from 'react-pose'

export default class WifiAp extends Component {
    constructor(props) {
        super(props);
        this.state = {
            name: "",
            signalStrength: 0,
            expanded: false
        }
    }

    componentDidMount() {
        this.setState({
            name: this.props.name,
            signalStrength: this.props.signalStrength
        });
    }
    
    render() {
        return (
            <ComponentWrap className="ap-element" >
                <ApWrap 
                    className="ap-wrapper" 
                    key={this.state.name} 
                    onClick={() => this.setState({expanded: !this.state.expanded})} 
                    pose={this.state.expanded ? "expand" : "collapse"}
                >
                    <WifiName>{this.state.name}</WifiName>
                    <IconWrapper pose={this.state.expanded ? "fadeOut" : "fadeIn"}>
                        <WifiIcon signalStrength={this.state.signalStrength} width="2.5em" height="2.5em"/>
                    </IconWrapper>
                </ApWrap>
                <PasswordInput ssid={this.state.name} expanded={this.state.expanded}/>
            </ComponentWrap>
        );
    }
}

const PasswordInput = (props) => {
    return (
        < InputWrap action="/login" method="POST" pose={ props.expanded ? "visible" : "hidden" }>
            <input type="hidden" name="ssid" value={props.ssid}/>
            <input type="text" name="password" placeholder="Password"/>
            <SubmitButton type="submit">
                Submit
            </SubmitButton>
        </InputWrap>
    )
}

/*<form style={{display: "flex", flexDirection: "row"}}>
    <input/><br/>
    <input type="submit" />
</form>*/

const expandProps = {
    expand: {
        height: "12em",
        transition: {
            type: "spring",
            stiffness: 200,
            damping: 15
        }
    },
    collapse: {
        height: "auto",
        transition: {
            type: "spring",
            stiffness: 200,
            damping: 15
        }
    }
}

const fadeProps = {
    fadeIn: {
        opacity: 1,
        transition: {
            delay: 150,
            duration: 200
        }
    },

    fadeOut: {
        opacity: 0,
    }
}

const passwordProps = {
    visible: {
        applyAtStart: {display: "block"},
        opacity: 1,
        transition: {
            delay: 150,
            duration: 200
        }
    },

    hidden: {
        applyAtStart: {display: "none"},
        opacity: 0,
    }
}

const ComponentWrap = styled.div`
    position: relative;
    display: "flex";
    flex-direction: "column";
    padding: 0.5em 0em;
    border-style: solid;
    border-color: ${props => props.theme.secondaryDark};
    border-width: 0.1em;
    background-color: ${props => props.theme.secondary};
`;

const ApWrap = styled(posed.div(expandProps))`
    display: flex;
    flex-direction: row;
    justify-content: space-between;
`;

const IconWrapper = styled(posed.div(fadeProps))`
    position: relative;
    right: 1em;
    top: 1.5em;
`;

const InputWrap = styled(posed.form(passwordProps))`
    display: flex;
    position: absolute;
    justify-content: space-between;
    padding: 2em;
    bottom: 1em;
`;

const PasswordWrap = styled.div`
    display: flex;
    flex-direction: row;
    justify-content: space-between;
`;

const SubmitButton = styled.button`
    background-color: ${props => props.theme.secondaryDark};
    border: none;
    width: 8em;
    height: 2em;
    position: relative;
    right: 2em;
`;

const Test = styled.div`
    height: 100px;
    background-color: red;
    display: block;
`;

const WifiName = styled.p`
    align-self: left;
    font-size: 1.7em;
    position: relative;
    left: 0.3em;
    color: ${props => props.theme.secondaryText};
`;

