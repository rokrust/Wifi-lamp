import React, {Component} from 'react'
import styled from 'styled-components'

export default class NavBar extends Component {
    render() {
        return (
            <NavBarWrapper>
                {this.props.children}
            </NavBarWrapper>
        )

    }
}

const NavBarWrapper = styled.div`
    display: flex;
    justify-content: flex-end;
    flex-direction: row;
    background-color: ${props => props.theme.primaryLight};
`;